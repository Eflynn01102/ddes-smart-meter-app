import { test, expect, describe } from "bun:test";
import { createHmacSignature } from "@/utils/hmac";

describe("createhmacSignature", () => {
  test("should create a valid HMAC signature", () => {
    const clientId = "testClient";
    const currentReading = "12345";
    const unix = "1625077765";

    const signature = createHmacSignature(clientId, currentReading, unix);

    expect(signature).toBeDefined();
    expect(signature).toMatch(/^[a-f0-9]{64}$/); // SHA-256 produces a 64-character hex string
  })
})
