package com.smartmeter.billing_service.http;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.web.reactive.function.client.WebClient;

import com.smartmeter.billing_service.domain.events.BillSnapshot;
import com.smartmeter.billing_service.domain.ports.BillPublisher;

@Component
public class HttpBillPublisher implements BillPublisher {
    private static final Logger log = LoggerFactory.getLogger(HttpBillPublisher.class);
    private final WebClient webClient;
    private final String clientUrl;

    public HttpBillPublisher(
            WebClient.Builder webClientBuilder,
            @Value("${client.bill_data.url:http://client:3000/bill_data}") String clientUrl) {
        this.webClient = webClientBuilder.build();
        this.clientUrl = clientUrl;
    }

    @Override
    public void publish(BillSnapshot snapshot) {
        var simpleBill = SimpleBillData.from(snapshot);
        webClient.post()
                .uri(clientUrl)
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(simpleBill)
                .retrieve()
                .bodyToMono(Void.class)
                .doOnSuccess(v -> log.info("Posted bill data to client: {}", clientUrl))
                .doOnError(e -> log.error("Failed to post bill data to client: {}", clientUrl, e))
                .subscribe();
    }
}
