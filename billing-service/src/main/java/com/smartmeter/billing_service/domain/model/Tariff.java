package com.smartmeter.billing_service.domain.model;

import java.math.BigDecimal;
import java.time.Instant;
import java.time.LocalTime;
import java.util.List;
import java.util.Objects;

/** Tariff descriptor with pricing models (Flat, Time-of-Use, Tiered). */
public record Tariff(
    String tariffId,
    Instant effectiveFrom,
    String currency,
    BigDecimal standingChargePerDay,
    BigDecimal taxPercent,
    PricingModel pricingModel
) {
  public Tariff {
    Objects.requireNonNull(tariffId);
    Objects.requireNonNull(effectiveFrom);
    Objects.requireNonNull(currency);
    Objects.requireNonNull(standingChargePerDay);
    Objects.requireNonNull(taxPercent);
    Objects.requireNonNull(pricingModel);
  }

  public sealed interface PricingModel permits FlatRate, TimeOfUse, Tiered {}

  /** Flat rate per kWh at all times. */
  public record FlatRate(BigDecimal ratePerKwh) implements PricingModel {
    public FlatRate { Objects.requireNonNull(ratePerKwh); }
  }

  /** Time-of-Use bands with optional fallback rate. */
  public record TimeOfUse(List<TimeBand> bands, BigDecimal fallbackRate) implements PricingModel {
    public TimeOfUse {
      Objects.requireNonNull(bands);
      Objects.requireNonNull(fallbackRate);
      if (bands.isEmpty()) throw new IllegalArgumentException("bands must not be empty");
    }
  }

  /** Tiered pricing by cumulative kWh in the billing period. */
  public record Tiered(List<Tier> tiers) implements PricingModel {
    public Tiered {
      Objects.requireNonNull(tiers);
      if (tiers.isEmpty()) throw new IllegalArgumentException("tiers must not be empty");
    }
  }

  public record TimeBand(LocalTime from, LocalTime to, BigDecimal ratePerKwh) {
    public TimeBand {
      Objects.requireNonNull(from); Objects.requireNonNull(to); Objects.requireNonNull(ratePerKwh);
      if (to.isBefore(from)) throw new IllegalArgumentException("Time band 'to' must be >= 'from'");
    }
  }

  /** If upToKwh == null the tier is open-ended (infinity). */
  public record Tier(BigDecimal upToKwh, BigDecimal ratePerKwh) {
    public Tier { Objects.requireNonNull(ratePerKwh); /* upToKwh may be null */ }
  }
}
