package com.smartmeter.billing_service.amqp;

import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.BindingBuilder;
import org.springframework.amqp.core.ExchangeBuilder;
import org.springframework.amqp.core.Queue;
import org.springframework.amqp.core.QueueBuilder;
import org.springframework.amqp.core.TopicExchange;
import org.springframework.amqp.rabbit.connection.ConnectionFactory;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.amqp.support.converter.Jackson2JsonMessageConverter;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class RabbitTopology {

  // Exchange names
  public static final String EX_READINGS = "smartmeter.readings";
  public static final String EX_BILLING  = "smartmeter.billing";

  // Ingestion events exchange and routing key
  public static final String EX_EVENTS = "events";
  public static final String RK_EVENTS_BILLING = "billing";

  // Queue for ingestion events
  public static final String Q_BILLING_EVENTS = "billing.events.q";

  // Routing keys
  public static final String RK_READING  = "meter.reading.v1";
  public static final String RK_BILL_ANY = "bill.updated.v1.*";

  // Our queues
  public static final String Q_BILLING_READINGS = "billing.readings.q";

  @Bean
  TopicExchange readingsExchange() { return ExchangeBuilder.topicExchange(EX_READINGS).durable(true).build(); }

  @Bean
  TopicExchange billingExchange() { return ExchangeBuilder.topicExchange(EX_BILLING).durable(true).build(); }

  @Bean
  TopicExchange eventsExchange() { return ExchangeBuilder.topicExchange(EX_EVENTS).durable(false).build(); }

  @Bean
  Queue billingReadingsQueue() { return QueueBuilder.durable(Q_BILLING_READINGS).build(); }

  @Bean
  Queue billingEventsQueue() { return QueueBuilder.durable(Q_BILLING_EVENTS).build(); }

  @Bean
  Binding readingsToBillingCalc() {
    return BindingBuilder.bind(billingReadingsQueue()).to(readingsExchange()).with(RK_READING);
  }

  @Bean
  Binding eventsToBilling() {
    return BindingBuilder.bind(billingEventsQueue()).to(eventsExchange()).with(RK_EVENTS_BILLING);
  }

  @Bean
  Jackson2JsonMessageConverter jackson2Converter() { return new Jackson2JsonMessageConverter(); }

  @Bean
  RabbitTemplate rabbitTemplate(ConnectionFactory cf, Jackson2JsonMessageConverter conv) {
    var tpl = new RabbitTemplate(cf);
    tpl.setMessageConverter(conv);
    return tpl;
  }
}
