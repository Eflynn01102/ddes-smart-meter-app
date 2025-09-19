package com.smartmeter.billing_service.infra.tariff;

import java.math.BigDecimal;

import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.Test;

import com.smartmeter.billing_service.domain.model.Tariff;

class FixedTariffResolverTest {
    @Test
    void testResolveReturnsConfiguredTariff() {
        Tariff tariff = new Tariff("id", java.time.Instant.EPOCH, "GBP", BigDecimal.ONE, BigDecimal.TEN, new Tariff.FlatRate(BigDecimal.valueOf(0.15)));
        FixedTariffResolver resolver = new FixedTariffResolver(tariff);
        assertSame(tariff, resolver.resolve(java.time.Instant.now()));
    }

    @Test
    void testFlatHelperCreatesFlatTariff() {
        FixedTariffResolver resolver = FixedTariffResolver.flat("id", "GBP", BigDecimal.ONE, BigDecimal.TEN, BigDecimal.valueOf(0.15));
        Tariff tariff = resolver.resolve(java.time.Instant.now());
        assertTrue(tariff.pricingModel() instanceof Tariff.FlatRate);
    }

    @Test
    void testTouHelperCreatesTouTariff() {
        FixedTariffResolver resolver = FixedTariffResolver.tou("id", "GBP", BigDecimal.ONE, BigDecimal.TEN);
        Tariff tariff = resolver.resolve(java.time.Instant.now());
        assertTrue(tariff.pricingModel() instanceof Tariff.TimeOfUse);
    }
}
