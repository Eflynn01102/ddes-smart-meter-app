package com.smartmeter.billing_service.domain.policy;

import java.math.BigDecimal;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

import com.smartmeter.billing_service.domain.value.Money;

class TaxPolicyTest {
    @Test
    void testComputeTax() {
        Money subtotal = new Money(BigDecimal.valueOf(100), "GBP");
        Money tax = TaxPolicy.computeTax(subtotal, BigDecimal.valueOf(5));
        assertEquals("GBP", tax.currency());
        assertEquals(new BigDecimal("5.00"), tax.amount().setScale(2));
    }
}
