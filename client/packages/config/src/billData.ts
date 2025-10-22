import { z } from "zod";

export type BillData =z.infer<typeof BillData>;
export const BillData = z.object({
  accountId: z.string(),
  periodStart: z.string(),
  currency: z.string(),
  energyCost: z.float32(),
  standingCharge: z.float32(),
  tax: z.string(),
  amountDue: z.float32(),
})
