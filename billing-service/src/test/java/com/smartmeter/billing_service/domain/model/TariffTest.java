package com.smartmeter.billing_service.domain.model;

import java.math.BigDecimal;
import java.time.Instant;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

class TariffTest {
    @Test
    void testTariffRecord() {
        Tariff.PricingModel model = new Tariff.FlatRate(BigDecimal.valueOf(0.12));
        Tariff tariff = new Tariff("tariff1", Instant.now(), "GBP", BigDecimal.ONE, BigDecimal.TEN, model);
        assertEquals("tariff1", tariff.tariffId());
        assertEquals("GBP", tariff.currency());
        assertEquals(BigDecimal.ONE, tariff.standingChargePerDay());
        assertEquals(BigDecimal.TEN, tariff.taxPercent());
        assertEquals(model, tariff.pricingModel());
    }

    @Test
    void testFlatRateModel() {
        Tariff.FlatRate flat = new Tariff.FlatRate(BigDecimal.valueOf(0.15));
        assertEquals(BigDecimal.valueOf(0.15), flat.ratePerKwh());
    }

    @Test
    void testTimeOfUseModel() {
        Tariff.TimeBand band = new Tariff.TimeBand(java.time.LocalTime.of(0,0), java.time.LocalTime.of(6,0), BigDecimal.valueOf(0.10));
        Tariff.TimeOfUse tou = new Tariff.TimeOfUse(List.of(band), BigDecimal.valueOf(0.20));
        assertEquals(1, tou.bands().size());
        assertEquals(BigDecimal.valueOf(0.20), tou.fallbackRate());
    }

    @Test
    void testTieredModel() {
        Tariff.Tier tier = new Tariff.Tier(BigDecimal.valueOf(100), BigDecimal.valueOf(0.08));
        Tariff.Tiered tiered = new Tariff.Tiered(List.of(tier));
        assertEquals(1, tiered.tiers().size());
    }
}
