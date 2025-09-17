package com.smartmeter.billing_service.domain.value;

public final class Ids {
  private Ids() {}

  public record AccountId(String value) {
    public static AccountId of(String v) { return new AccountId(v); }
    @Override public String toString(){ return value; }
  }

  public record MeterId(String value) {
    public static MeterId of(String v) { return new MeterId(v); }
    @Override public String toString(){ return value; }
  }

  public record ReadingId(String value) {
    public static ReadingId of(String v) { return new ReadingId(v); }
    @Override public String toString(){ return value; }
  }
}
