import { z } from "zod";

export const HMACSecert = z.string().parse(process.env.HMAC);