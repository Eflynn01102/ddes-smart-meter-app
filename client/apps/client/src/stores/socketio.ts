import { io, type Socket } from "socket.io-client";
import { defineStore } from "pinia";
import { computed, ref, watch } from "vue";
import type {
	ClientToServerEvents,
	ServerToClientEvents,
	SocketAlter,
	SocketData,
} from "@client/config/src/index";

export const useSocketStore = defineStore("socketio", () => {

	const billData = ref<SocketData>()

	const historicalBillDate = ref<Date>()

	const historicalBillData = ref<SocketData>()

	const alterMessage = ref<SocketAlter>();

	const socket: Socket<ServerToClientEvents, ClientToServerEvents> = io("/");

	const isSocketActive = computed(() => socket.active);

	socket.on("connect", () => {
		console.log("Connected to server with ID:", socket.id);
	});

	socket.emit("hello");

	watch(historicalBillDate, (date) => {
		if (!date) return;
		socket.emit("request_historical_bill_data", date.toISOString());
	})

	socket.on("bill_data", (data: SocketData) => {
		console.log("Received bill data from server:");
		billData.value = data;
	})

	socket.on("historical_bill_data", (data: SocketData) => {
		console.log("Received historical bill data from server");
		historicalBillData.value = data;
	})

	socket.on("alert", (message: SocketAlter) => {
		console.log("Server alter message:", message.clientId);
		alterMessage.value = message;
	})

	socket.on("connect_error", (err) => {
		console.log(err.name);
		console.log(err.message);
	});

	return { billData, alterMessage, isSocketActive };
});
