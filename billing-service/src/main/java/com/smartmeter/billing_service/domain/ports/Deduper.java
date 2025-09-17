package com.smartmeter.billing_service.domain.ports;

import com.smartmeter.billing_service.domain.value.Ids.AccountId;
import com.smartmeter.billing_service.domain.value.Ids.ReadingId;

public interface Deduper {
  /** @return true if this (account, readingId) was not seen before and is now marked. */
  boolean tryMark(AccountId account, ReadingId readingId);
}
