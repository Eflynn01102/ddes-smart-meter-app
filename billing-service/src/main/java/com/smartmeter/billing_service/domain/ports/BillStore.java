package com.smartmeter.billing_service.domain.ports;

import com.smartmeter.billing_service.domain.model.Bill;
import com.smartmeter.billing_service.domain.value.Ids.AccountId;

public interface BillStore {
  Bill load(AccountId id);
  void save(AccountId id, Bill bill);
}
