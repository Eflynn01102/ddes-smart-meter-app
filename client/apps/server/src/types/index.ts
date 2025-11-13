import { z } from "zod";
import { BillData } from "@client/config/src/billData";
import { title } from "node:process";

export type APIAlterType = z.infer<typeof APIAlterType>;
export const APIAlterType = z.object({
	clientId: z.string(),
	title: z.string(),
	severity: z.string(),
	message: z.string(),
});

export type APIBillData = z.infer<typeof APIBillData>;
export const APIBillData = BillData
