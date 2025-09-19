package com.smartmeter.billing_service.amqp;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import org.junit.jupiter.api.Test;

class ReadingMessageTest {
    @Test
    void testDefaultConstructorAndFields() {
        ReadingMessage msg = new ReadingMessage();
        assertEquals(1, msg.schema);
        assertNull(msg.readingId);
        assertNull(msg.accountId);
        assertNull(msg.meterId);
        assertNull(msg.timestamp);
        assertEquals(0.0, msg.cumulativeKwh);
        assertNull(msg.signature);
    }
}
