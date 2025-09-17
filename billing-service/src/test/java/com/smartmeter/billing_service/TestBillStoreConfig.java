package com.smartmeter.billing_service;

import org.springframework.boot.test.context.TestConfiguration;
import org.springframework.context.annotation.Bean;

import com.smartmeter.billing_service.domain.model.Bill;
import com.smartmeter.billing_service.domain.ports.BillStore;
import com.smartmeter.billing_service.domain.value.Ids.AccountId;

@TestConfiguration
public class TestBillStoreConfig {
    @Bean
    public BillStore billStore() {
        return new BillStore() {
            @Override
            public Bill load(AccountId id) {
                // Return a dummy Bill for testing
                return null;
            }

            @Override
            public void save(AccountId id, Bill bill) {
                // No-op for testing
            }
        };
    }

        @Bean
        public com.smartmeter.billing_service.domain.ports.TariffResolver tariffResolver() {
            return at -> null; // Return null or dummy Tariff for testing
        }

    @Bean
    public com.smartmeter.billing_service.domain.ports.Deduper deduper() {
        return (account, readingId) -> true; // Always mark as new for testing
    }
}
