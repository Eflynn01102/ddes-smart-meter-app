/*
package com.smartmeter.billing_service.amqp;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;

@Component
public class EventsBillingListener {
    private static final Logger log = LoggerFactory.getLogger(EventsBillingListener.class);

    @RabbitListener(queues = RabbitTopology.Q_BILLING_EVENTS)
    public void handleBillingEvent(String message) {
        // TODO: Replace String with your event DTO if known
        log.info("Received billing event from ingestion: {}", message);
        // Add business logic to process the event here
    }
}
*/
