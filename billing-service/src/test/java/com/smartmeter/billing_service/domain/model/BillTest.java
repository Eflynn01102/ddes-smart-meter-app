package com.smartmeter.billing_service.domain.model;

import java.time.Instant;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

import com.smartmeter.billing_service.domain.value.Ids;
import com.smartmeter.billing_service.domain.value.Money;

class BillTest {
    @Test
    void testBillInitialization() {
        Ids.AccountId accountId = Ids.AccountId.of("acc1");
        Instant start = Instant.now();
        Bill bill = new Bill(accountId, start, "GBP");
        assertEquals(accountId, bill.getAccountId());
        assertEquals(start, bill.getPeriodStart());
        assertEquals("GBP", bill.getCurrency());
        assertEquals(Money.zero("GBP"), bill.getEnergyCost());
        assertEquals(Money.zero("GBP"), bill.getStandingCharge());
        assertEquals(Money.zero("GBP"), bill.getTax());
        assertEquals(Money.zero("GBP"), bill.getAmountDue());
    }
}
