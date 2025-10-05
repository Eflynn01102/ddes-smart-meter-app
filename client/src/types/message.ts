import { z } from "zod";

export type rabbitMessage = z.infer<typeof rabbitMessage>;
export const rabbitMessage = z.object({
  clientID: z.string(),
  currentReading: z.int(),
  unix: z.number(),
  fwVersion: z.string(),
  unit: z.string(),
  signature: z.string(),
})