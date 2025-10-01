package com.smartmeter.billing_service.config;

import java.time.Clock;
import java.time.ZoneId;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.smartmeter.billing_service.domain.ports.BillStore;
import com.smartmeter.billing_service.domain.service.BillingCalculator;
import com.smartmeter.billing_service.infra.store.InMemoryBillStore;

@Configuration
public class CoreConfig {
  private static final Logger log = LoggerFactory.getLogger(CoreConfig.class);

  public CoreConfig() {
    log.info("CoreConfig loaded: BillStore bean will be registered");
  }

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
  Clock clock() { return Clock.systemUTC(); }

  @Bean
  public BillStore billStore(Clock clock, ZoneId zoneId, @Value("${billing.currency:GBP}") String currency) {
    return new InMemoryBillStore(clock, zoneId, currency);
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
