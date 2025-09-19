package com.smartmeter.billing_service.api;

import java.util.Map;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

class HealthProbeControllerTest {
    @Test
    void testHealthzReturnsOk() {
        HealthProbeController controller = new HealthProbeController();
        Map<String, Object> result = controller.healthz();
        assertEquals("ok", result.get("status"));
    }
}
