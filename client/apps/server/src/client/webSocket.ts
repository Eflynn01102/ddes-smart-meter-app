import { createServer } from "node:http";
import type {
	ClientToServerEvents,
	InterServerEvents,
	ServerToClientEvents,
	SocketData,
	SocketAlter,
} from "@client/config/src/index";
import express from "express";
import { Server } from "socket.io";
import { ref } from "vue";
import bodyParser from "body-parser";
import { APIAlterType, APIBillData } from "../types";

const app = express();
export const server = createServer(app);
export const port = 3000;

const io = new Server<
	ClientToServerEvents,
	ServerToClientEvents,
	InterServerEvents,
	SocketData
>(server, {
	cors: {
		origin: "*",
		methods: ["GET", "POST"],
	},
});

const socketIds = ref<string>();

io.on("connection", (socket) => {
	console.log("a user connected");

	socketIds.value = socket.id;

	socket.on("hello", () => {
		console.log("Received 'hello' from client:", socket.id);
	});

	socket.on("disconnect", () => {
		console.log("user disconnected");
	});
});

io.engine.on("connection_error", (err) => {
	console.log("Error code", err.code); // the error code, for example 1
	console.log("Error message", err.message);
	console.log("Error Context", err.context);
});

app.use(bodyParser.json());

app.get("/hello-world", (req, res) => {
	console.log("Hello World endpoint hit");
	res.send("Hello World!");
})

app.post("/bill_data", (req, res) => {
	const data = APIBillData.safeParse(req.body);
	if (!data.success)
		return res.status(400).json({ error: "Invalid data format" });

	console.log("Received bill data:", data.data);
	sendDataToAllClients("bill_data", data.data );

	res.status(200).json({ status: "Data sent to clients" });
});

app.post("/alter", (req, res) => {
	const data = APIAlterType.safeParse(res.json());
	if (!data.success)
		return res.status(400).json({ error: "Invalid data format" });
	console.log("Received alert data:", data.data);
	sendDataToAllClients("alert", data.data);
	res.status(200).json({ status: "Alert sent to clients" });
})

async function fetchHistoricalBillData(data: string) {
	const url = "http://billServer"
	const res = await fetch(url, {
		method: "POST",
		body: JSON.stringify({ date: data }),
		headers: {
			"Content-Type": "application/json",
		},
	})
	if (!res.ok) {
		throw new Error("Failed to fetch historical bill data");
	}
	const billData: SocketData = await res.json();
	sendDataToAllClients("historical_bill_data", billData);
}

/*
This function sends data to all connected clients. If a specific socket is provided, it sends the data to that socket instead.
*/
function sendDataToAllClients<
	E extends keyof ServerToClientEvents
>(
	event: E,
	data: Parameters<ServerToClientEvents[E]>[0]

) {
	if (!io) {
		console.error("Socket.io server is not initialized.");
		return;
	}

	io.emit(event, ...([data] as Parameters<ServerToClientEvents[E]>));
	console.log("Sent data to client");
}
