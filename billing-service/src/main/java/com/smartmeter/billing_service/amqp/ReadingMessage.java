package com.smartmeter.billing_service.amqp;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;

/** Wire DTO for meter.reading.v1 */
@JsonIgnoreProperties(ignoreUnknown = true)
public class ReadingMessage {
  public String readingId;
  public String accountId;
  public String meterId;
  public String timestamp;      // ISO-8601 (UTC)
  public double cumulativeKwh;  // cumulative energy
  public String signature;      // optional
  public int schema = 1;

  public ReadingMessage() {}
}
