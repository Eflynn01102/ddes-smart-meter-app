package com.smartmeter.billing_service.config;

import com.smartmeter.billing_service.domain.service.BillingCalculator;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.time.ZoneId;

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
}
