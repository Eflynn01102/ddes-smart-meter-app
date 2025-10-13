import { z } from "zod";

export type BillData =z.infer<typeof BillData>;
export const BillData = z.object({
  unitsUsed: z.number(),
  price: z.number(),
  units: z.string(),
  date: z.date()
})