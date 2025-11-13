import { z } from "zod";
import { BillData } from "@client/config/src/billData";

export type APIAlterType = z.infer<typeof APIAlterType>;
export const APIAlterType = z.object({
	clientId: z.string(),
	message: z.string(),
});

export type APIBillData = z.infer<typeof APIBillData>;
export const APIBillData = BillData
