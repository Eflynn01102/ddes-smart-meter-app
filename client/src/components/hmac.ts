import crypto from "node:crypto"
import { HMACSecert } from "../types/env";

export function createHmacSignature(clientId: string, currentReading: string, unix: string): string {
  const data = `${clientId}${currentReading}${unix}`;
  const hmacSecert = crypto.createHmac("sha256", HMACSecert).update(data)
  return hmacSecert.digest("hex");
}    