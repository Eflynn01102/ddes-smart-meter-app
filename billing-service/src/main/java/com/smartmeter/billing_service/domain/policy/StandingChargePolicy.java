package com.smartmeter.billing_service.domain.policy;

import java.math.BigDecimal;

import com.smartmeter.billing_service.domain.value.Money;

public final class StandingChargePolicy {
  private StandingChargePolicy() {}

  public static Money forDays(String currency, BigDecimal ratePerDay, int days) {
    var total = ratePerDay.multiply(BigDecimal.valueOf(days));
    return new Money(total, currency).rounded2();
  }
}
