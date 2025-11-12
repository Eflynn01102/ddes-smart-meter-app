import { defineStore } from "pinia";
import { ref, computed, watch } from "vue";
import { useRouter } from "vue-router";
import { useToast } from "primevue/usetoast";
import { useSocketStore } from "@/stores/socketio";

export const useAuthStore = defineStore("auth", () => {
	const router = useRouter();
	const toast = useToast();

	const socketStore = useSocketStore();

	const isLoggedIn = ref(false);

	const knownUsername = computed(() => socketStore.validUser?.userName || "");
	const knownRole = computed(() => socketStore.validUser?.roles || "");
	const knownPassword = computed(() => socketStore.validUser?.password || "");
	const knownClientId = computed(() => socketStore.validUser?.clientId || "");

	const validUsername = ref("");
	const validRole = ref("");
	const validPassword = ref("");
	const validClientId = ref("");

	function loginHandler(userName: string, password: string) {
		if (userName === "" || password === "") {
			console.log("empty fields");
			toast.add({
				severity: "error",
				summary: "Login Failed",
				detail: "Invalid username or password",
				life: 3000,
			});
			return;
		}

		socketStore.requestUser({ userName, password });

		setTimeout(() => {
			if (
				userName === knownUsername.value &&
				password === knownPassword.value
			) {
				validUsername.value = knownUsername.value;
				validRole.value = knownRole.value;
				validPassword.value = knownPassword.value;
				validClientId.value = knownClientId.value;
				isLoggedIn.value = true;
				router.push({ name: "bill" });
				toast.add({
					severity: "success",
					summary: "Login Successful",
					detail: `Welcome back, ${validUsername.value}!`,
					life: 3000,
				});
			} else {
				isLoggedIn.value = false;
				toast.add({
					severity: "error",
					summary: "Login Failed",
					detail: "Invalid username or password",
					life: 3000,
				});
			}
		}, 500);
	}

	function logoutHandler() {
		isLoggedIn.value = false;
		validUsername.value = "";
		validRole.value = "";
		validPassword.value = "";
		router.push({ name: "login" });
		toast.add({
			severity: "info",
			summary: "Logged Out",
			detail: "You have been logged out successfully",
			life: 3000,
		});
	}

	return { isLoggedIn, knownRole, knownClientId, loginHandler, logoutHandler };
});
