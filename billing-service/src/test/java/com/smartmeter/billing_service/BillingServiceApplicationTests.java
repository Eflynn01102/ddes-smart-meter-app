package com.smartmeter.billing_service;

import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.context.annotation.Import;

@Import(TestBillStoreConfig.class)
@SpringBootTest
class BillingServiceApplicationTests {

	@Test
	void contextLoads() {
	}

}
