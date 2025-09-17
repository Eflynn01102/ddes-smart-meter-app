package com.smartmeter.billing_service.domain.value;

import java.time.Duration;
import java.time.Instant;

public record Interval(Instant from, Instant to) {
  public Interval {
    if (from == null || to == null || !to.isAfter(from)) {
      throw new IllegalArgumentException("Invalid interval: from=" + from + ", to=" + to);
    }
  }
  public Duration duration() { return Duration.between(from, to); }
}
