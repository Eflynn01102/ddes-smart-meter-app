package com.smartmeter.billing_service.domain.service;

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.time.Duration;
import java.time.Instant;
import java.time.LocalTime;
import java.time.ZoneId;
import java.util.List;

import com.smartmeter.billing_service.domain.model.Bill;
import com.smartmeter.billing_service.domain.model.BillLineItem;
import com.smartmeter.billing_service.domain.model.Tariff;
import com.smartmeter.billing_service.domain.policy.StandingChargePolicy;
import com.smartmeter.billing_service.domain.policy.TaxPolicy;
import com.smartmeter.billing_service.domain.value.Ids.ReadingId;
import com.smartmeter.billing_service.domain.value.Interval;
import com.smartmeter.billing_service.domain.value.Money;

/** Core algorithm that updates a Bill with a new cumulative meter reading. */
public final class BillingCalculator {
  private final ZoneId zone;

  public BillingCalculator(ZoneId zone) { this.zone = zone; }

  /**
   * Apply a new reading to the bill (idempotency should be enforced by caller via ReadingId).
   * @param state current bill state (mutated and returned)
   * @param rid reading id (stored onto the state)
   * @param ts reading timestamp
   * @param cumulativeKwh cumulative kWh reported by the meter
   * @param tariff tariff to use at this time
   */
  public Bill applyReading(Bill state, ReadingId rid, Instant ts, BigDecimal cumulativeKwh, Tariff tariff) {
    var pointer = state.pointer();
    if (pointer == null) {
      state.advancePointer(new Bill.ReadingPointer(ts, cumulativeKwh));
      state.setLastReadingId(rid);
      return state; // first reading starts the period; no delta
    }

    // Delta kWh (guard against meter resets/backward)
    var deltaKwh = cumulativeKwh.subtract(pointer.cumulativeKwh());
    if (deltaKwh.compareTo(BigDecimal.ZERO) < 0) deltaKwh = BigDecimal.ZERO;

    var interval = new Interval(pointer.ts(), ts);
    var totalSec = BigDecimal.valueOf(interval.duration().toSeconds());
    if (totalSec.signum() <= 0) {
      // identical timestamp: nothing to accrue, just move the pointer
      state.advancePointer(new Bill.ReadingPointer(ts, cumulativeKwh));
      state.setLastReadingId(rid);
      return state;
    }

    // Split interval at typical boundaries (00:00, 07:00, 17:00, 22:00)
    var segmenter = new IntervalSegmenter(zone);
    var boundaries = List.of(LocalTime.MIDNIGHT, LocalTime.of(7,0), LocalTime.of(17,0), LocalTime.of(22,0));
    var slices = segmenter.splitByClockHours(interval, boundaries);

    var rateResolver = new TariffRateResolver(tariff, zone);
    var energyCostAcc = Money.zero(tariff.currency());
    var consumedAcc = BigDecimal.ZERO;

    for (var s : slices) {
      var sliceSec = BigDecimal.valueOf(s.duration().toSeconds());
      var fraction = sliceSec.divide(totalSec, 8, RoundingMode.HALF_UP);
      var sliceKwh = deltaKwh.multiply(fraction);
      var rate = rateResolver.rateFor(s.from(), state.consumedKwh().add(consumedAcc));
      var cost = new Money(sliceKwh.multiply(rate), tariff.currency()).rounded2();

      state.addLineItem(new BillLineItem.EnergyItem(s.from(), s.to(), sliceKwh, rate, cost));
      energyCostAcc = energyCostAcc.plus(cost);
      consumedAcc = consumedAcc.add(sliceKwh);
    }

    state.addEnergy(consumedAcc, energyCostAcc);

    // Standing charge accrual since last pointer (whole days difference – already accrued)
    var daysElapsed = (int) Duration.between(pointer.ts(), ts).toDays();
    var newDays = Math.max(0, daysElapsed - state.standingAccruedDays());
    if (newDays > 0) {
      var perDay = tariff.standingChargePerDay();
      var cost = StandingChargePolicy.forDays(tariff.currency(), perDay, newDays);
      state.addLineItem(new BillLineItem.StandingItem(newDays, new Money(perDay, tariff.currency()), cost));
      state.addStanding(cost, newDays);
    }

    // Tax and totals
    var subtotal = state.energyCost().plus(state.standingCharge());
    var tax = TaxPolicy.computeTax(subtotal, tariff.taxPercent());
    var total = subtotal.plus(tax).rounded2();
    state.setTaxAndTotal(tax, total);

    // Advance pointer & mark last reading
    state.advancePointer(new Bill.ReadingPointer(ts, cumulativeKwh));
    state.setLastReadingId(rid);
    return state;
  }
}
