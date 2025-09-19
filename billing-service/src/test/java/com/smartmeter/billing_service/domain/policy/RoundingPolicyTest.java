package com.smartmeter.billing_service.domain.policy;

import java.math.BigDecimal;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

class RoundingPolicyTest {
    @Test
    void testKwhRounding() {
        BigDecimal value = new BigDecimal("1.23456");
        assertEquals(new BigDecimal("1.235"), RoundingPolicy.kwh(value));
    }

    @Test
    void testRateRounding() {
        BigDecimal value = new BigDecimal("0.123456");
        assertEquals(new BigDecimal("0.1235"), RoundingPolicy.rate(value));
    }
}
