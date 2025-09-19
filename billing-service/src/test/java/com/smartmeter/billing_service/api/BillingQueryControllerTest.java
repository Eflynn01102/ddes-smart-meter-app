package com.smartmeter.billing_service.api;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import org.junit.jupiter.api.Test;
import org.mockito.Mockito;
import org.springframework.http.ResponseEntity;

import com.smartmeter.billing_service.domain.events.BillSnapshot;
import com.smartmeter.billing_service.domain.ports.BillStore;
import com.smartmeter.billing_service.domain.value.Ids;

class BillingQueryControllerTest {
    @Test
    void testCurrentReturnsSnapshot() {
        BillStore store = Mockito.mock(BillStore.class);
        var bill = new com.smartmeter.billing_service.domain.model.Bill(
            com.smartmeter.billing_service.domain.value.Ids.AccountId.of("acc123"),
            java.time.Instant.now(),
            "GBP"
        );
        Mockito.when(store.load(Ids.AccountId.of("acc123"))).thenReturn(bill);
        BillingQueryController controller = new BillingQueryController(store);
        ResponseEntity<BillSnapshot> response = controller.current("acc123");
        assertEquals(200, response.getStatusCodeValue());
        assertNotNull(response.getBody());
    }
}
