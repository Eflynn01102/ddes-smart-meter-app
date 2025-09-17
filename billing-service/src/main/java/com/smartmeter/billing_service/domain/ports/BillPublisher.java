package com.smartmeter.billing_service.domain.ports;

import com.smartmeter.billing_service.domain.events.BillSnapshot;

public interface BillPublisher {
  void publish(BillSnapshot snapshot);
}
