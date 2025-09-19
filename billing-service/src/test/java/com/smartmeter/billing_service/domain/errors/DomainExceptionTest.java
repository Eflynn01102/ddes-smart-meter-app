package com.smartmeter.billing_service.domain.errors;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

class DomainExceptionTest {
    @Test
    void testMessageConstructor() {
        DomainException ex = new DomainException("error");
        assertEquals("error", ex.getMessage());
    }

    @Test
    void testMessageAndCauseConstructor() {
        Throwable cause = new RuntimeException("cause");
        DomainException ex = new DomainException("error", cause);
        assertEquals("error", ex.getMessage());
        assertEquals(cause, ex.getCause());
    }
}
