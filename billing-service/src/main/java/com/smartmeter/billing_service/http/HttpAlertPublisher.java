package com.smartmeter.billing_service.http;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.web.reactive.function.client.WebClient;

@Component
public class HttpAlertPublisher {
    private static final Logger log = LoggerFactory.getLogger(HttpAlertPublisher.class);
    private final WebClient webClient;
    private final String alertUrl;

    public HttpAlertPublisher(
            WebClient.Builder webClientBuilder,
            @Value("${alert.url:http://server:3000/alert}") String alertUrl) {
        this.webClient = webClientBuilder.build();
        this.alertUrl = alertUrl;
    }

    public void sendAlert(String clientId, String title, String severity, String message) {
        var payload = new AlertPayload(clientId, title, severity, message);
        webClient.post()
                .uri(alertUrl)
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(payload)
                .retrieve()
                .bodyToMono(Void.class)
                .doOnSuccess(v -> log.info("Posted alert to {}: {}", alertUrl, payload))
                .doOnError(e -> log.error("Failed to post alert to {}: {}", alertUrl, payload, e))
                .subscribe();
    }

    public record AlertPayload(String clientId, String title, String severity, String message) {}
}
