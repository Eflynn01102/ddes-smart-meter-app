package com.smartmeter.billing_service.domain.model;

import java.math.BigDecimal;
import java.time.Instant;

import com.smartmeter.billing_service.domain.value.Money;

/** Itemized entries contributing to the bill total. */
public sealed interface BillLineItem permits BillLineItem.EnergyItem, BillLineItem.StandingItem {

  record EnergyItem(Instant tsFrom, Instant tsTo, BigDecimal kwh, BigDecimal ratePerKwh, Money cost)
      implements BillLineItem {}

  record StandingItem(int days, Money ratePerDay, Money cost) implements BillLineItem {}
}
