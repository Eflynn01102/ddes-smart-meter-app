import dotenv from "dotenv";
import { z } from "zod";

dotenv.config();

const EnvSchema = z.object({
	HMAC: z.string(),
});

export const envSecret = EnvSchema.parse(process.env);
