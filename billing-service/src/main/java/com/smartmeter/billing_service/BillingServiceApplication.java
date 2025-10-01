package com.smartmeter.billing_service;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.ComponentScan;

@SpringBootApplication
@ComponentScan(basePackages = {
    "com.smartmeter.billing_service",
    "com.smartmeter.billing_service.config"
})
public class BillingServiceApplication {
  public static void main(String[] args) {
    SpringApplication.run(BillingServiceApplication.class, args);
  }
}
