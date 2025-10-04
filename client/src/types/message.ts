import { z } from "zod";

export type rabbitMessage = z.infer<typeof rabbitMessage>;
export const rabbitMessage = z.object({
  clientID: z.string(),
  currentReading: z.number(),
  unix: z.number(),
  fwVersion: z.string(),
  unit: z.string(),
  encryption: z.string(),
})