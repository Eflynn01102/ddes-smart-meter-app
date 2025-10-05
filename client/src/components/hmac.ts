import crypto from "node:crypto"
import { envSecert } from "../env";

export function createHmacSignature(clientId: string, currentReading: string, unix: string): string {
  const data = `${clientId}${currentReading}${unix}`;
  try {
    const hmacSecert = crypto.createHmac("sha256", envSecert.HMAC).update(data)
    return hmacSecert.digest("hex");
  } catch (error) {
    console.error("Error creating HMAC signature:", error);
    throw new Error("Failed to create HMAC signature");
  }
}    

// createHmacSignature("client-1", "43", "1672531199");

