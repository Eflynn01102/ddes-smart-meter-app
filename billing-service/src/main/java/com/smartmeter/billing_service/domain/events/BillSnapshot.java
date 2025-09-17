package com.smartmeter.billing_service.domain.events;

import java.math.BigDecimal;
import java.time.Instant;
import java.util.List;

import com.smartmeter.billing_service.domain.model.Bill;
import com.smartmeter.billing_service.domain.model.BillLineItem;

/** Read/Publish DTO shaped for HTTP/AMQP. */
public record BillSnapshot(
    String accountId,
    Instant periodStart,
    Instant periodEnd,
    String currency,
    Totals totals,
    List<LineItem> lineItems,
    String lastReadingId
) {
  public record Totals(
      BigDecimal consumedKwh,
      double energyCost,
      double standingCharge,
      double tax,
      double amountDue
  ) {}

  public sealed interface LineItem permits Energy, Standing {}

  public record Energy(Instant tsFrom, Instant tsTo, BigDecimal kwh, double rate, double cost) implements LineItem {}
  public record Standing(int days, double ratePerDay, double cost) implements LineItem {}

  public static BillSnapshot from(Bill bill) {
    var items = bill.lineItems().stream().map(li -> switch (li) {
      case BillLineItem.EnergyItem e -> new Energy(
          e.tsFrom(), e.tsTo(), e.kwh(),
          e.ratePerKwh().doubleValue(),
          e.cost().amount().doubleValue());
      case BillLineItem.StandingItem s -> new Standing(
          s.days(),
          s.ratePerDay().amount().doubleValue(),
          s.cost().amount().doubleValue());
    }).toList();

    var totals = new Totals(
        bill.consumedKwh(),
        bill.energyCost().amount().doubleValue(),
        bill.standingCharge().amount().doubleValue(),
        bill.tax().amount().doubleValue(),
        bill.amountDue().amount().doubleValue()
    );

        return new BillSnapshot(
            bill.accountId().value(),
            bill.periodStart(),
            bill.periodEnd(),
            bill.currency(),
            totals,
            (List<LineItem>) (List<?>) items,
            bill.lastReadingId() == null ? null : bill.lastReadingId().value()
        );
  }
}
