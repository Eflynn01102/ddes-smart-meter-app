import { z } from "zod";
import dotenv from "dotenv";

dotenv.config();

const EnvSchema = z.object({
  HMAC: z.string()
})

export const envSecret = EnvSchema.parse(process.env);