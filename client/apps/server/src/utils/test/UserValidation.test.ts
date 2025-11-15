import { test, expect, describe } from "bun:test";
import { validateUser } from "../userValidation";

describe("User Validation Tests", () => {
  test("should validate a user and return valid as true", async () => {
    const userName = "admin";
    const password = "password123";
    const result = await validateUser(userName, password);
    expect(result.valid).toBe(true);
    expect(result.user.userName).toBe(userName);
    expect(result.user.roles).toBe("admin");
    expect(result.user.clientId).toBe("client-131");
    expect(result.user.password).toBe(password);
  })

  test("should no validate user and return valid as false", async () => {
    const userName = "bob";
    const password = "bobsBurgers";
    const result = await validateUser(userName, password);
    expect(result.valid).toBe(false);
    expect(result.user.userName).toBe("");
    expect(result.user.roles).toBe("");
    expect(result.user.clientId).toBe("");
    expect(result.user.password).toBe("");
  })
})
