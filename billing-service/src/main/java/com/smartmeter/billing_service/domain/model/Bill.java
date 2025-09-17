package com.smartmeter.billing_service.domain.model;

import java.math.BigDecimal;
import java.time.Instant;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import com.smartmeter.billing_service.domain.value.Ids.AccountId;
import com.smartmeter.billing_service.domain.value.Ids.ReadingId;
import com.smartmeter.billing_service.domain.value.Money;

/** Aggregate for the current billing period state. */
public final class Bill {

  private final AccountId accountId;
  private final Instant periodStart;
  private Instant periodEnd; // nullable for an open period
  private final String currency;

  private final List<BillLineItem> lineItems = new ArrayList<>();

  private BigDecimal consumedKwh = BigDecimal.ZERO;
  private Money energyCost;
  private Money standingCharge;
  private Money tax;
  private Money amountDue;

  private ReadingId lastReadingId;
  private ReadingPointer pointer; // last cumulative reading ref
  private int standingAccruedDays = 0;

  public Bill(AccountId accountId, Instant periodStart, String currency) {
    this.accountId = Objects.requireNonNull(accountId);
    this.periodStart = Objects.requireNonNull(periodStart);
    this.currency = Objects.requireNonNull(currency);
    this.energyCost = Money.zero(currency);
    this.standingCharge = Money.zero(currency);
    this.tax = Money.zero(currency);
    this.amountDue = Money.zero(currency);
  }

  /** Immutable pointer to last known reading (ts, cumulative kWh). */
  public record ReadingPointer(Instant ts, BigDecimal cumulativeKwh) {}

  // Mutations (invoked only by domain services)
  public void addLineItem(BillLineItem item) { lineItems.add(item); }

  public void addEnergy(BigDecimal kwh, Money cost) {
    this.consumedKwh = this.consumedKwh.add(kwh);
    this.energyCost = this.energyCost.plus(cost);
  }

  public void addStanding(Money cost, int days) {
    this.standingCharge = this.standingCharge.plus(cost);
    this.standingAccruedDays += days;
  }

  public void setTaxAndTotal(Money tax, Money total) {
    this.tax = tax; this.amountDue = total;
  }

  public void advancePointer(ReadingPointer rp) { this.pointer = rp; }
  public void setLastReadingId(ReadingId rid) { this.lastReadingId = rid; }
  public void closePeriod(Instant end) { this.periodEnd = end; }

  // Getters
  public AccountId accountId() { return accountId; }
  public Instant periodStart() { return periodStart; }
  public Instant periodEnd() { return periodEnd; }
  public String currency() { return currency; }
  public List<BillLineItem> lineItems() { return List.copyOf(lineItems); }
  public BigDecimal consumedKwh() { return consumedKwh; }
  public Money energyCost() { return energyCost; }
  public Money standingCharge() { return standingCharge; }
  public Money tax() { return tax; }
  public Money amountDue() { return amountDue; }
  public ReadingId lastReadingId() { return lastReadingId; }
  public ReadingPointer pointer() { return pointer; }
  public int standingAccruedDays() { return standingAccruedDays; }
}
