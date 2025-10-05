import { z } from "zod";
import dotenv from "dotenv";

dotenv.config();

const EnvSchema = z.object({
  HMAC: z.string()
})

export const envSecert = EnvSchema.parse(process.env);