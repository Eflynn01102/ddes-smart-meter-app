import { createServer } from "node:http";
import { BillData } from "@client/config/src/billData";
import type {
	ClientToServerEvents,
	InterServerEvents,
	ServerToClientEvents,
	SocketData,
} from "@client/config/src/index";
import express from "express";
import { Server, type Socket } from "socket.io";
import { ref } from "vue";

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

// app.post("/bill_data", (req, res) => {
// 	const data = BillData.safeParse(res.json());
// 	if (!data.success)
// 		return res.status(400).json({ error: "Invalid data format" });
// 	sendDataToAllClients({
// 		clientId: socketIds.value || "no-id",
// 		data: data.data,
// 	});
// 	res.status(200).json({ status: "Data sent to clients" });
// });

/*
This function sends data to all connected clients. If a specific socket is provided, it sends the data to that socket instead.
*/
function sendDataToAllClients(
	data: SocketData,
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
	if (!socketServer.value && !socketToClient) {
		console.log("No clients connected");
		return;
	}

	if (!socketToClient) {
		if (!socketServer.value) {
			console.log("No clients connected");
			return;
		}
		socket = socketServer.value;
	} else {
		socket = socketToClient;
	}

	socket.emit("data", data);
	console.log("Sent data to client");
}
