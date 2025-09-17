package com.smartmeter.billing_service.domain.policy;

import java.math.BigDecimal;
import java.math.RoundingMode;

import com.smartmeter.billing_service.domain.value.Money;

public final class TaxPolicy {
  private TaxPolicy() {}
  /** percent is e.g. 5.0 for 5%. */
  public static Money computeTax(Money subtotal, BigDecimal percent) {
    var tax = subtotal.amount()
        .multiply(percent)
        .divide(BigDecimal.valueOf(100), 4, RoundingMode.HALF_UP);
    return new Money(tax, subtotal.currency()).rounded2();
  }
}
