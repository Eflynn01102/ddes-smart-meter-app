package com.smartmeter.billing_service.domain.policy;

import java.math.BigDecimal;
import java.math.RoundingMode;

/** Centralized rounding rules to keep tests deterministic. */
public final class RoundingPolicy {
  private RoundingPolicy() {}
  public static BigDecimal kwh(BigDecimal v) { return v.setScale(3, RoundingMode.HALF_UP); }
  public static BigDecimal rate(BigDecimal v) { return v.setScale(4, RoundingMode.HALF_UP); }
}
