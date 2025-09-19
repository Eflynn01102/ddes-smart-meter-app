package com.smartmeter.billing_service.domain.policy;

import java.math.BigDecimal;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

import com.smartmeter.billing_service.domain.value.Money;

class StandingChargePolicyTest {
    @Test
    void testForDaysCalculation() {
        Money result = StandingChargePolicy.forDays("GBP", BigDecimal.valueOf(0.5), 4);
        assertEquals("GBP", result.currency());
        assertEquals(new BigDecimal("2.00"), result.amount().setScale(2));
    }
}
