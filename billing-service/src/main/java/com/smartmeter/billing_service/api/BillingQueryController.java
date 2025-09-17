package com.smartmeter.billing_service.api;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.smartmeter.billing_service.domain.events.BillSnapshot;
import com.smartmeter.billing_service.domain.ports.BillStore;
import com.smartmeter.billing_service.domain.value.Ids;

@RestController
@RequestMapping("/billing")
public class BillingQueryController {

  private final BillStore store;

  public BillingQueryController(BillStore store) { this.store = store; }

  @GetMapping("/{accountId}/current")
  public ResponseEntity<BillSnapshot> current(@PathVariable String accountId) {
    var bill = store.load(Ids.AccountId.of(accountId));
    return ResponseEntity.ok(BillSnapshot.from(bill));
  }
}
