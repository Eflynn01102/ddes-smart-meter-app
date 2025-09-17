package com.smartmeter.billing_service.amqp;

import com.smartmeter.billing_service.domain.events.BillSnapshot;
import com.smartmeter.billing_service.domain.model.Bill;
import com.smartmeter.billing_service.domain.ports.BillPublisher;
import com.smartmeter.billing_service.domain.ports.BillStore;
import com.smartmeter.billing_service.domain.ports.Deduper;
import com.smartmeter.billing_service.domain.ports.TariffResolver;
import com.smartmeter.billing_service.domain.service.BillingCalculator;
import com.smartmeter.billing_service.domain.value.Ids;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.messaging.handler.annotation.Payload;
import org.springframework.stereotype.Component;

import java.math.BigDecimal;
import java.time.Instant;
import java.time.ZoneId;

import static com.smartmeter.billing_service.amqp.RabbitTopology.Q_BILLING_READINGS;

@Component
public class ReadingListener {
  private static final Logger log = LoggerFactory.getLogger(ReadingListener.class);

  private final BillingCalculator calc;
  private final BillStore store;
  private final TariffResolver tariffs;
  private final Deduper deduper;
  private final BillPublisher publisher;
  private final ZoneId zone;

  public ReadingListener(
      BillingCalculator calc,
      BillStore store,
      TariffResolver tariffs,
      Deduper deduper,
      BillPublisher publisher,
      ZoneId zone) {
    this.calc = calc; this.store = store; this.tariffs = tariffs; this.deduper = deduper; this.publisher = publisher; this.zone = zone;
  }

  @RabbitListener(queues = Q_BILLING_READINGS)
  public void onReading(@Payload ReadingMessage msg) {
    try {
      var accountId = Ids.AccountId.of(msg.accountId);
      var rid = Ids.ReadingId.of(msg.readingId);
      if (!deduper.tryMark(accountId, rid)) {
        log.debug("Duplicate reading ignored account={} readingId={}", msg.accountId, msg.readingId);
        return;
      }

      var bill = store.load(accountId);
      var ts = Instant.parse(msg.timestamp);
      var tariff = tariffs.resolve(ts);

      Bill updated = calc.applyReading(
          bill, rid, ts, BigDecimal.valueOf(msg.cumulativeKwh), tariff
      );

      store.save(accountId, updated);
      publisher.publish(BillSnapshot.from(updated));
      log.info("Processed reading account={} readingId={} kwh={} total={}",
               msg.accountId, msg.readingId, msg.cumulativeKwh, updated.amountDue().amount());
    } catch (Exception e) {
      log.error("Failed to process reading: {}", msg, e);
      // in a prod system, you might DLQ or nack here; Spring defaults to requeue=false on listener fatal errors if configured.
      throw e;
    }
  }
}
