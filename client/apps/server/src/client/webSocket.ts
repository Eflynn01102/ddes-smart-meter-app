import { createServer } from "node:http";
import { BillData } from "@client/config/src/billData";
import type {
	ClientToServerEvents,
	InterServerEvents,
	ServerToClientEvents,
	SocketData,
	SocketAlter,
} from "@client/config/src/index";
import express from "express";
import { Server, Socket } from "socket.io";
import { ref } from "vue";
import { APIAlterType, APIBillData } from "../types";

const app = express();
export const server = createServer(app);
export const port = 3000;

const io = new Server<
	ClientToServerEvents,
	ServerToClientEvents,
	InterServerEvents,
	SocketData
>(server);

const socketServer =
	ref<
		Socket<
			ClientToServerEvents,
			ServerToClientEvents,
			InterServerEvents,
			SocketData
		>
	>();

const socketIds = ref<string>();

io.on("connection", (socket) => {
	console.log("a user connected");

	socketServer.value = socket;
	socketIds.value = socket.id;

	// socket.emit("data",
	//   {
	//     clientId: socket.id,
	//     data: {
	//       unitsUsed: 100,
	//       price: 50,
	//       units: "kWh",
	//       date: new Date()
	//     }
	//   }
	// ); // Send initial data

	socket.on("hello", () => {
		console.log("Received 'hello' from client:", socket.id);
	});

	socket.on("request_historical_Bill_data", (date: string) => {
		console.log(
			"Client requested historical bill data for date:",
			date);
		return {
			clientId: "4",
			data: {
				accountId: "123456",
				periodStart: new Date(date).toISOString(),
				currency: "USD",
				energyCost: 75.5,
				standingCharge: 10.0,
				tax: "6",
				amountDue: 91.5
			}
		}
	})

	socket.on("disconnect", () => {
		console.log("user disconnected");
	});
});

io.engine.on("connection_error", (err) => {
	console.log("Error code", err.code); // the error code, for example 1
	console.log("Error message", err.message);
	console.log("Error Context", err.context);
});

app.get("/hello-world", (req, res) => {
	console.log("Hello World endpoint hit");
	res.send("Hello World!");
})

app.post("/bill_data", (req, res) => {
	const data = APIBillData.safeParse(res.json());
	if (!data.success)
		return res.status(400).json({ error: "Invalid data format" });
	sendDataToAllClients(data.data, "bill_data");
	res.status(200).json({ status: "Data sent to clients" });
});

app.post("/alter", (req, res) => {
	const data = APIAlterType.safeParse(res.json());
	if (!data.success)
		return res.status(400).json({ error: "Invalid data format" });
	sendDataToAllClients(data.data, "alert");
	res.status(200).json({ status: "Alert sent to clients" });
})

/*
This function sends data to all connected clients. If a specific socket is provided, it sends the data to that socket instead.
*/
function sendDataToAllClients<
	E extends keyof ServerToClientEvents
>(
	data: Parameters<ServerToClientEvents[E]>[0],
	event: E,
	socketToClient?: Socket<
		ClientToServerEvents,
		ServerToClientEvents,
		InterServerEvents,
		SocketData
	>,
) {
	let socket: Socket<
		ClientToServerEvents,
		ServerToClientEvents,
		InterServerEvents,
		SocketData
	>;

	if (!socketToClient) {
		if (!socketServer.value) return;
		socket = socketServer.value;
	} else {
		socket = socketToClient;
	}

	socket.emit(event, ...([data] as Parameters<ServerToClientEvents[E]>));
	console.log("Sent data to client");
}
