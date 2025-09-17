package com.smartmeter.billing_service.amqp;

import com.smartmeter.billing_service.domain.events.BillSnapshot;
import com.smartmeter.billing_service.domain.ports.BillPublisher;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.stereotype.Component;

import static com.smartmeter.billing_service.amqp.RabbitTopology.EX_BILLING;

@Component
public class AmqpBillPublisher implements BillPublisher {

  private final RabbitTemplate rabbit;

  public AmqpBillPublisher(RabbitTemplate rabbit) { this.rabbit = rabbit; }

  @Override
  public void publish(BillSnapshot snapshot) {
    // route per account for targeted subscriptions: bill.updated.v1.{accountId}
    var routingKey = "bill.updated.v1." + snapshot.accountId();
    rabbit.convertAndSend(EX_BILLING, routingKey, snapshot);
  }
}
