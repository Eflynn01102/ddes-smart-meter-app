package com.smartmeter.billing_service.domain.ports;

import com.smartmeter.billing_service.domain.model.Tariff;

import java.time.Instant;

public interface TariffResolver {
  /** Resolve the tariff that applies at a given instant. */
  Tariff resolve(Instant at);
}
