import { io, type Socket } from "socket.io-client";
import { defineStore } from "pinia";
import { computed, ref } from "vue";
import { useAuthStore } from "@/stores/auth";
import type {
	ClientToServerEvents,
	ServerToClientEvents,
	SocketAlter,
	SocketData,
	SocketValidUser,
	SocketUnknownUser,
	SocketMeter,
} from "@client/config/src/index";
import { useToast } from "primevue/usetoast";

export const useSocketStore = defineStore("socketio", () => {
	const toast = useToast();
	const authStore = useAuthStore();

	const billData = ref<SocketData>();
	const currentUsage = ref<SocketMeter>();
	const historicalBillData = ref<SocketData>();

	const alterMessage = ref<SocketAlter>();
	const validUser = ref<SocketValidUser>();

	const socket: Socket<ServerToClientEvents, ClientToServerEvents> = io("/");
	const isSocketActive = computed(() => socket.active);

	socket.on("connect", () => {
		console.log(
			"Connected to server with ID:",
			socket.id,
			new Date().toISOString(),
		);
	});

	socket.emit("hello");

	function requestHistoricalBillData(date: string) {
		socket.emit("request_historical_bill_data", date);
	}

	function requestUser(user: SocketUnknownUser) {
		socket.emit("request_user", user);
	}

	socket.on("current_usage", (usage: SocketMeter) => {
		console.log("Received current usage from server:", usage);
		currentUsage.value = usage;
	});

	socket.on("bill_data", (data: SocketData) => {
		console.log("Received bill data from server:", data);
		if (data.accountId === authStore.knownClientId) billData.value = data;
		if (data.accountId === "client-131") billData.value = data;
	});

	socket.on("historical_bill_data", (data: SocketData) => {
		console.log("Received historical bill data from server");
		if (data.accountId === authStore.knownClientId) historicalBillData.value = data;
		if (data.accountId === "client-131") historicalBillData.value = data;
	});

	socket.on("valid_user", (user: SocketValidUser) => {
		console.log("Received valid user from server:", user);
		validUser.value = user;
	});

	socket.on("alert", (message: SocketAlter) => {
		console.log("Server alter message:", message);
		if (message.clientId === authStore.knownClientId) {
			alterMessage.value = message;
			toast.add({
				severity: "info",
				summary: "Alert",
				detail: message.message,
				life: 3000,
			});
		}
		if (message.clientId === "client-131") {
			alterMessage.value = message;
			toast.add({
				severity: "info",
				summary: "Alert",
				detail: `${message.clientId} - ${message.message}`,
				life: 3000,
			});
		}
	});

	socket.on("connect_error", (err) => {
		console.log(err.name);
		console.log(err.message);
	});

	return {
		billData,
		currentUsage,
		historicalBillData,
		alterMessage,
		validUser,
		isSocketActive,
		requestHistoricalBillData,
		requestUser,
	};
});
