import { test, expect, describe } from "bun:test";
import { generateRandomNumber, generateRandomNumberInRange } from "@/utils/randomNumberGen";

describe("Random Number Generator", () => {
  test("should generate a random number", () => {
    const maxNumber = 10
    const randomNumber = generateRandomNumber(maxNumber);
    expect(typeof randomNumber).toBe("number");
    expect(randomNumber).toBeGreaterThanOrEqual(0);
    expect(randomNumber).toBeLessThan(maxNumber);
  });

  test("should generate a random number within a specified range", () => {
    const minNumber = 5;
    const maxNumber = 15;
    const randomNumberInRange = generateRandomNumberInRange(minNumber, maxNumber);
    expect(typeof randomNumberInRange).toBe("number");
    expect(randomNumberInRange).toBeGreaterThanOrEqual(minNumber);
    expect(randomNumberInRange).toBeLessThan(maxNumber);
  });
})
