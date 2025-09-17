package com.smartmeter.billing_service.domain.service;

import com.smartmeter.billing_service.domain.model.Tariff;

import java.math.BigDecimal;
import java.time.Instant;
import java.time.ZoneId;

/** Resolves the applicable rate at a moment, given a tariff and cumulative kWh so far. */
public final class TariffRateResolver {
  private final Tariff tariff; private final ZoneId zone;

  public TariffRateResolver(Tariff tariff, ZoneId zone) {
    this.tariff = tariff; this.zone = zone;
  }

  public BigDecimal rateFor(Instant ts, BigDecimal cumulativeUsedKwhSoFar) {
    var pm = tariff.pricingModel();
    return switch (pm) {
      case Tariff.FlatRate fr -> fr.ratePerKwh();
      case Tariff.TimeOfUse tou -> {
        var lt = ts.atZone(zone).toLocalTime();
        var band = tou.bands().stream()
            .filter(b -> !lt.isBefore(b.from()) && !lt.isAfter(b.to()))
            .findFirst().orElse(null);
        yield band != null ? band.ratePerKwh() : tou.fallbackRate();
      }
      case Tariff.Tiered tiered -> {
        var k = cumulativeUsedKwhSoFar;
        var tiers = tiered.tiers();
        var match = tiers.stream()
            .filter(t -> t.upToKwh() == null || k.compareTo(t.upToKwh()) <= 0)
            .findFirst().orElse(tiers.get(tiers.size() - 1));
        yield match.ratePerKwh();
      }
      default -> throw new IllegalStateException("Unknown pricing model: " + pm);
    };
  }
}
