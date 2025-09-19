package com.smartmeter.billing_service.amqp;

import org.junit.jupiter.api.Test;
import org.mockito.Mockito;
import org.springframework.amqp.rabbit.core.RabbitTemplate;

import com.smartmeter.billing_service.domain.events.BillSnapshot;

class AmqpBillPublisherTest {
    @Test
    void testPublishBillSnapshot() {
        RabbitTemplate rabbitTemplate = Mockito.mock(RabbitTemplate.class);
        AmqpBillPublisher publisher = new AmqpBillPublisher(rabbitTemplate);
        BillSnapshot snapshot = Mockito.mock(BillSnapshot.class);
        Mockito.when(snapshot.accountId()).thenReturn("testAccount");
        publisher.publish(snapshot);
        Mockito.verify(rabbitTemplate).convertAndSend(
            Mockito.eq(com.smartmeter.billing_service.amqp.RabbitTopology.EX_BILLING),
            Mockito.eq("bill.updated.v1.testAccount"),
            Mockito.eq(snapshot)
        );
    }
}
