export function generateRandomNumber(max: number): number {
  return Math.floor(Math.random() * max)
}

export function generateRandomNumberInRange(min: number, max: number): number {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

const number = generateRandomNumberInRange(10, 100);
console.log(number);

const number2 = generateRandomNumberInRange(number, 100)
console.log(number2);

const number3 = generateRandomNumberInRange(number2, 100)
console.log(number3);

const number4 = generateRandomNumberInRange(number3, 100)
console.log(number4);