package com.smartmeter.billing_service.http;

import com.smartmeter.billing_service.domain.events.BillSnapshot;

/**
 * Simplified bill data DTO matching the client's expected schema.
 * Maps from the full BillSnapshot to the format expected by client:3000/bill_data
 */
public record SimpleBillData(
    String accountId,
    String periodStart,
    String currency,
    float energyCost,
    float standingCharge,
    String tax,
    float amountDue
) {
    public static SimpleBillData from(BillSnapshot snapshot) {
        return new SimpleBillData(
            snapshot.accountId(),
            snapshot.periodStart().toString(),
            snapshot.currency(),
            (float) snapshot.totals().energyCost(),
            (float) snapshot.totals().standingCharge(),
            String.format("%.2f", snapshot.totals().tax()),
            (float) snapshot.totals().amountDue()
        );
    }
}
