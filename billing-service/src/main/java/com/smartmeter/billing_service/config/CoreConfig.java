package com.smartmeter.billing_service.config;

import java.math.BigDecimal;
import java.time.Clock;
import java.time.ZoneId;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.smartmeter.billing_service.domain.ports.BillStore;
import com.smartmeter.billing_service.domain.service.BillingCalculator;
import com.smartmeter.billing_service.infra.store.InMemoryBillStore;
import com.smartmeter.billing_service.infra.tariff.FixedTariffResolver;

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
  @ConditionalOnMissingBean(BillStore.class)
  public BillStore billStore(Clock clock, ZoneId zoneId, @Value("${billing.currency:GBP}") String currency) {
    log.info("Registering BillStore bean via CoreConfig (InMemoryBillStore)");
    return new InMemoryBillStore(clock, zoneId, currency);
  }

  @Bean
  public com.smartmeter.billing_service.domain.ports.TariffResolver tariffResolver(
      @Value("${billing.currency:GBP}") String currency) {
    // Production tariff with flat rate: £0.25/kWh, standing charge £0.50/day, 5% tax
    return FixedTariffResolver.flat(
        "default-tariff",
        currency,
        new BigDecimal("0.50"),  // standing charge per day
        new BigDecimal("5.00"),  // tax percentage
        new BigDecimal("0.25")   // rate per kWh
    );
  }

  @Bean
  public com.smartmeter.billing_service.domain.ports.Deduper deduper() {
    return (account, readingId) -> true; // Dummy implementation for production context
  }
}
