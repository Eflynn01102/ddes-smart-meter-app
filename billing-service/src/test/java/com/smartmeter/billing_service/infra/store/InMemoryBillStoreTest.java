package com.smartmeter.billing_service.infra.store;

import java.time.Clock;
import java.time.ZoneId;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import org.junit.jupiter.api.Test;

import com.smartmeter.billing_service.domain.model.Bill;
import com.smartmeter.billing_service.domain.value.Ids;

class InMemoryBillStoreTest {
    @Test
    void testLoadCreatesNewBill() {
        InMemoryBillStore store = new InMemoryBillStore(Clock.systemUTC(), ZoneId.of("Europe/London"), "GBP");
        Ids.AccountId id = Ids.AccountId.of("acc1");
        Bill bill = store.load(id);
        assertNotNull(bill);
        assertEquals("acc1", bill.getAccountId().value());
        assertEquals("GBP", bill.getCurrency());
    }

    @Test
    void testSaveAndLoadReturnsSameBill() {
        InMemoryBillStore store = new InMemoryBillStore(Clock.systemUTC(), ZoneId.of("Europe/London"), "GBP");
        Ids.AccountId id = Ids.AccountId.of("acc2");
        Bill bill = new Bill(id, java.time.Instant.now(), "GBP");
        store.save(id, bill);
        Bill loaded = store.load(id);
        assertSame(bill, loaded);
    }
}
