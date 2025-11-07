import { users } from "../../config/config.json";
export async function validateUser(userName: string, password: string) {
	const user = users.find((user) => user.userName === userName);
	if (!user) {
		return {
			valid: false,
			user: {
				userName: "",
				roles: "",
				password: "",
			},
		};
	}
	if (user.password !== password) {
		return {
			valid: false,
			user: {
				userName: "",
				roles: "",
				password: "",
			},
		};
	}
	return { valid: true, user };
}
