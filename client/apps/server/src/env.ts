import dotenv from "dotenv";
import { z } from "zod";

dotenv.config();

const EnvSchema = z.object({
	HMAC: z.string(),
	RabbitMQAddress: z.string().default("127.0.0.1")
});

export const envSecret = EnvSchema.parse(process.env);
