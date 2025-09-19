package com.smartmeter.billing_service.domain.model;

import java.math.BigDecimal;
import java.time.Instant;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

import com.smartmeter.billing_service.domain.value.Money;

class BillLineItemTest {
    @Test
    void testEnergyItemRecord() {
        Instant from = Instant.now();
        Instant to = from.plusSeconds(3600);
        BigDecimal kwh = BigDecimal.valueOf(10.5);
        BigDecimal rate = BigDecimal.valueOf(0.15);
        Money cost = Money.zero("GBP");
        BillLineItem.EnergyItem item = new BillLineItem.EnergyItem(from, to, kwh, rate, cost);
        assertEquals(from, item.tsFrom());
        assertEquals(to, item.tsTo());
        assertEquals(kwh, item.kwh());
        assertEquals(rate, item.ratePerKwh());
        assertEquals(cost, item.cost());
    }

    @Test
    void testStandingItemRecord() {
        Money rate = Money.zero("GBP");
        Money cost = Money.zero("GBP");
        BillLineItem.StandingItem item = new BillLineItem.StandingItem(5, rate, cost);
        assertEquals(5, item.days());
        assertEquals(rate, item.ratePerDay());
        assertEquals(cost, item.cost());
    }
}
