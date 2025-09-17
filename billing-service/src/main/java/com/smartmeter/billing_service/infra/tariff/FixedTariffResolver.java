package com.smartmeter.billing_service.infra.tariff;

import com.smartmeter.billing_service.domain.model.Tariff;
import com.smartmeter.billing_service.domain.ports.TariffResolver;

import java.math.BigDecimal;
import java.time.Instant;
import java.time.LocalTime;
import java.util.List;
import java.util.Objects;

/** Returns a preconfigured Tariff; perfect for dev/tests. */
public class FixedTariffResolver implements TariffResolver {
  private final Tariff tariff;

  public FixedTariffResolver(Tariff tariff) { this.tariff = Objects.requireNonNull(tariff); }

  @Override public Tariff resolve(Instant at) { return tariff; }

  // Helpers
  public static FixedTariffResolver flat(String id, String ccy, BigDecimal standingPerDay, BigDecimal taxPct, BigDecimal rate) {
    return new FixedTariffResolver(new Tariff(id, Instant.EPOCH, ccy, standingPerDay, taxPct, new Tariff.FlatRate(rate)));
  }

  public static FixedTariffResolver tou(String id, String ccy, BigDecimal standingPerDay, BigDecimal taxPct) {
    var bands = List.of(
        new Tariff.TimeBand(LocalTime.of(0,0),  LocalTime.of(16,59,59), new BigDecimal("0.20")),
        new Tariff.TimeBand(LocalTime.of(17,0), LocalTime.of(21,59,59), new BigDecimal("0.30")),
        new Tariff.TimeBand(LocalTime.of(22,0), LocalTime.of(23,59,59), new BigDecimal("0.22"))
    );
    return new FixedTariffResolver(new Tariff(id, Instant.EPOCH, ccy, standingPerDay, taxPct, new Tariff.TimeOfUse(bands, new BigDecimal("0.25"))));
  }
}
