package com.smartmeter.billing_service.infra.store;

import java.time.Clock;
import java.time.LocalDate;
import java.time.ZoneId;
import java.util.Objects;
import java.util.concurrent.ConcurrentHashMap;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

import com.smartmeter.billing_service.domain.model.Bill;
import com.smartmeter.billing_service.domain.ports.BillStore;
import com.smartmeter.billing_service.domain.value.Ids.AccountId;

/** Thread-safe, no-IO store; creates a new Bill on first load for an account. */
@Component
public class InMemoryBillStore implements BillStore {
  private final ConcurrentHashMap<String, Bill> map = new ConcurrentHashMap<>();
  private final Clock clock;
  private final ZoneId zone;
  private final String currency;

  public InMemoryBillStore(Clock clock, ZoneId zone, @Value("${billing.currency:GBP}") String currency) {
    this.clock = Objects.requireNonNull(clock);
    this.zone = Objects.requireNonNull(zone);
    this.currency = Objects.requireNonNull(currency);
  }

  @Override
  public Bill load(AccountId id) {
    return map.computeIfAbsent(id.value(), k -> {
      // Use clock to derive current period start (midnight in target zone)
      var periodStart = LocalDate.now(clock.withZone(zone)).atStartOfDay(zone).toInstant();
      return new Bill(id, periodStart, currency);
    });
  }

  @Override
  public void save(AccountId id, Bill bill) {
    map.put(id.value(), bill);
  }
}
