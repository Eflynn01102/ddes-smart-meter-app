package com.smartmeter.billing_service.api;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.Map;

@RestController
public class HealthProbeController {
  @GetMapping("/healthz")
  public Map<String, Object> healthz() {
    return Map.of("status","ok");
  }
}
