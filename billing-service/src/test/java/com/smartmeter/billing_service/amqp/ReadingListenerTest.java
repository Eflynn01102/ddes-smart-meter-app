package com.smartmeter.billing_service.amqp;

import java.time.ZoneId;

import org.junit.jupiter.api.Test;
import org.mockito.Mockito;

import com.smartmeter.billing_service.domain.ports.BillPublisher;
import com.smartmeter.billing_service.domain.ports.BillStore;
import com.smartmeter.billing_service.domain.ports.Deduper;
import com.smartmeter.billing_service.domain.ports.TariffResolver;
import com.smartmeter.billing_service.domain.service.BillingCalculator;

class ReadingListenerTest {
    @Test
    void testConstructorAndDependencies() {
        BillingCalculator calc = Mockito.mock(BillingCalculator.class);
        BillStore store = Mockito.mock(BillStore.class);
        TariffResolver tariffs = Mockito.mock(TariffResolver.class);
        Deduper deduper = Mockito.mock(Deduper.class);
        BillPublisher publisher = Mockito.mock(BillPublisher.class);
        ZoneId zone = ZoneId.of("Europe/London");
        ReadingListener listener = new ReadingListener(calc, store, tariffs, deduper, publisher, zone);
        // Basic instantiation test
        assert listener != null;
    }
}
