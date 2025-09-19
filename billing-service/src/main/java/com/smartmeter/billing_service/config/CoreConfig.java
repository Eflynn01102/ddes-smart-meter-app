package com.smartmeter.billing_service.config;

import java.time.ZoneId;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.smartmeter.billing_service.domain.service.BillingCalculator;

@Configuration
public class CoreConfig {

  @Bean
  ZoneId zoneId() {
    // Billing assumes UK time when segmenting TOU day boundaries
    return ZoneId.of("Europe/London");
  }

  @Bean
  BillingCalculator billingCalculator(ZoneId zoneId) {
    return new BillingCalculator(zoneId);
  }

  @Bean
  public com.smartmeter.billing_service.domain.ports.BillStore billStore() {
    return new com.smartmeter.billing_service.domain.ports.BillStore() {
      @Override
      public com.smartmeter.billing_service.domain.model.Bill load(com.smartmeter.billing_service.domain.value.Ids.AccountId id) {
        // Dummy implementation for production context
        return null;
      }
      @Override
      public void save(com.smartmeter.billing_service.domain.value.Ids.AccountId id, com.smartmeter.billing_service.domain.model.Bill bill) {
        // No-op
      }
    };
  }

  @Bean
  public com.smartmeter.billing_service.domain.ports.TariffResolver tariffResolver() {
    return accountId -> null; // Dummy implementation for production context
  }

  @Bean
  public com.smartmeter.billing_service.domain.ports.Deduper deduper() {
    return (account, readingId) -> true; // Dummy implementation for production context
  }
}
