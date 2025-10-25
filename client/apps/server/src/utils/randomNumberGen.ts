export function generateRandomNumber(max: number): number {
	return Math.floor(Math.random() * max);
}

export function generateRandomNumberInRange(min: number, max: number): number {
	return Math.floor(Math.random() * (max - min + 1)) + min;
}
