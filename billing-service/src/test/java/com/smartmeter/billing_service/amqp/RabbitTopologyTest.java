package com.smartmeter.billing_service.amqp;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import org.junit.jupiter.api.Test;
import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.Queue;
import org.springframework.amqp.core.TopicExchange;
import org.springframework.amqp.support.converter.Jackson2JsonMessageConverter;

class RabbitTopologyTest {
    @Test
    void testExchangeBeans() {
        RabbitTopology topology = new RabbitTopology();
        TopicExchange readingsExchange = topology.readingsExchange();
        TopicExchange billingExchange = topology.billingExchange();
        assertEquals(RabbitTopology.EX_READINGS, readingsExchange.getName());
        assertEquals(RabbitTopology.EX_BILLING, billingExchange.getName());
    }

    @Test
    void testQueueBean() {
        RabbitTopology topology = new RabbitTopology();
        Queue queue = topology.billingReadingsQueue();
        assertEquals(RabbitTopology.Q_BILLING_READINGS, queue.getName());
    }

    @Test
    void testBindingBean() {
        RabbitTopology topology = new RabbitTopology();
        Binding binding = topology.readingsToBillingCalc();
        assertEquals(RabbitTopology.RK_READING, binding.getRoutingKey());
    }

    @Test
    void testJacksonConverterBean() {
        RabbitTopology topology = new RabbitTopology();
        Jackson2JsonMessageConverter converter = topology.jackson2Converter();
        assertNotNull(converter);
    }
}
