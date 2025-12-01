package com.smartmeter.billing_service.config;

import java.time.Clock;
import java.time.ZoneId;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.Test;

import com.smartmeter.billing_service.domain.ports.BillStore;
import com.smartmeter.billing_service.domain.ports.Deduper;
import com.smartmeter.billing_service.domain.ports.TariffResolver;
import com.smartmeter.billing_service.domain.service.BillingCalculator;

class CoreConfigTest {
    @Test
    void testZoneIdBean() {
        CoreConfig config = new CoreConfig();
        ZoneId zoneId = config.zoneId();
        assertEquals("Europe/London", zoneId.getId());
    }

    @Test
    void testBillingCalculatorBean() {
        CoreConfig config = new CoreConfig();
        BillingCalculator calc = config.billingCalculator(config.zoneId());
        assertNotNull(calc);
    }

    @Test
    void testBillStoreBean() {
        CoreConfig config = new CoreConfig();
        Clock clock = Clock.systemDefaultZone();
        ZoneId zoneId = config.zoneId();
        String outputDir = "test-output";
        BillStore store = config.billStore(clock, zoneId, outputDir);
        assertNotNull(store);
    }

    @Test
    void testTariffResolverBean() {
        CoreConfig config = new CoreConfig();
        TariffResolver resolver = config.tariffResolver();
        assertNotNull(resolver);
        assertNull(resolver.resolve(null));
    }

    @Test
    void testDeduperBean() {
        CoreConfig config = new CoreConfig();
        Deduper deduper = config.deduper();
        assertNotNull(deduper);
        // Should always return true for dummy implementation
        assertTrue(deduper.tryMark(null, null));
    }
}
