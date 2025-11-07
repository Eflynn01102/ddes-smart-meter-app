import { z } from "zod";
import { zodToJsonSchema } from "zod-to-json-schema";

export type Config = z.infer<typeof Config>;
export const Config = z.object(z.object({
  users:
    z.object({
      user1: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user2: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user3: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user4: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user5: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user6: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user7: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user8: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user9: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user10: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user11: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
      user12: z.object({
        name: z.string(),
        role: z.enum(['admin', 'user']),
        password: z.string().min(8),
      }),
    })
  }))

if(import.meta.url === new URL("./config.ts", import.meta.url).href) {
  try {
    const configSchema = zodToJsonSchema(Config);
    Bun.write("./config/config.schema.json", JSON.stringify(configSchema));
  } catch (error) {
    console.error("Error generating JSON schema:", error);
  }
}

export default Config;
