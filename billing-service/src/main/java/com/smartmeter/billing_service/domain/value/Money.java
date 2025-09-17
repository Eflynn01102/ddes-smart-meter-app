package com.smartmeter.billing_service.domain.value;

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.Objects;

/** Small money type with fixed currency and sane rounding helpers. */
public record Money(BigDecimal amount, String currency) {

  public Money {
    Objects.requireNonNull(amount, "amount");
    Objects.requireNonNull(currency, "currency");
    amount = amount.setScale(4, RoundingMode.HALF_UP); // keep extra precision internally
  }

  public static Money zero(String ccy) { return new Money(BigDecimal.ZERO, ccy); }

  public Money plus(Money other) { ensureSameCcy(other); return new Money(this.amount.add(other.amount), currency); }
  public Money minus(Money other) { ensureSameCcy(other); return new Money(this.amount.subtract(other.amount), currency); }
  public Money times(BigDecimal multiplier) { return new Money(this.amount.multiply(multiplier), currency); }
  public Money rounded2() { return new Money(this.amount.setScale(2, RoundingMode.HALF_UP), currency); }

  private void ensureSameCcy(Money other) {
    if (!this.currency.equals(other.currency)) throw new IllegalArgumentException("Currency mismatch: " + currency + " vs " + other.currency);
  }
}
