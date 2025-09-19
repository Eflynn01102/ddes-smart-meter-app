package com.smartmeter.billing_service.security;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import org.junit.jupiter.api.Test;
import org.springframework.context.annotation.Profile;

class SecurityConfigProdTest {
    @Test
    void testClassAnnotations() {
        Profile profile = SecurityConfigProd.class.getAnnotation(Profile.class);
        assertNotNull(profile);
        assertArrayEquals(new String[]{"prod"}, profile.value());
    }
}
