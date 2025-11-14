import type {
	ClientToServerEvents,
	InterServerEvents,
	ServerToClientEvents,
	SocketData,
	SocketUnknownUser,
} from "@client/config/src/index";
import { Server, type Socket } from "socket.io";
import type { createServer } from "node:http";
import { validateUser } from "@/utils/userValidation";

export class WebSocket {
	static instance: WebSocket;

	public static ioClient: Server<
		ClientToServerEvents,
		ServerToClientEvents,
		InterServerEvents,
		SocketData
	>;

	private static _socket: Socket<ClientToServerEvents, ServerToClientEvents>;

	private constructor() {}

	public static getInstance(): WebSocket {
		if (!WebSocket.instance) {
			WebSocket.instance = new WebSocket();
		}
		return WebSocket.instance;
	}

	public static get socket() {
		return WebSocket._socket;
	}

	public intializeWebSocket(server: ReturnType<typeof createServer>) {
		WebSocket.ioClient = new Server<
			ClientToServerEvents,
			ServerToClientEvents,
			InterServerEvents,
			SocketData
		>(server);

		WebSocket.ioClient.on("connection", (socket) => {
			console.log("a user connected");

			WebSocket._socket = socket;

			socket.on("hello", () => {
				console.log("Received 'hello' from client:", socket.id);
			});

			socket.on("request_historical_bill_data", (date: string) => {
				console.log("Client requested historical bill data for date:", date);
				try {
					this.fetchHistoricalBillData(date);
				} catch (error) {
					console.error("Error fetching historical bill data:", error);
				}
			});

			socket.on("request_user", async (user: SocketUnknownUser) => {
				console.log("Client requested user validation");
				const valUser = await validateUser(user.userName, user.password);
				if (!valUser.valid) {
					console.log("Invalid user credentials");
					this.sendDataToClient("valid_user", valUser.user);
					return;
				}
				this.sendDataToClient("valid_user", valUser.user);
			});

			socket.on("disconnect", () => {
				console.log("user disconnected");
			});
		});

		WebSocket.ioClient.engine.on("connection_error", (err) => {
			console.log("Error code", err.code); // the error code, for example 1
			console.log("Error message", err.message);
			console.log("Error Context", err.context);
		});
	}

	private async fetchHistoricalBillData(data: string) {
		const url = "http://billServer";
		const res = await fetch(url, {
			method: "POST",
			body: JSON.stringify({ date: data }),
			headers: {
				"Content-Type": "application/json",
			},
		});
		if (!res.ok) {
			throw new Error("Failed to fetch historical bill data");
		}
		const billData: SocketData = await res.json();
		this.sendDataToClient("historical_bill_data", billData);
	}

	public sendDataToClient<K extends keyof ServerToClientEvents>(
		event: K,
		...data: Parameters<ServerToClientEvents[K]>
	) {
		if (!WebSocket._socket) {
			console.error("No client connected to send data");
			return;
		}
		WebSocket._socket.emit(event, ...data);
		console.log(`Sent event '${event}' to client with data`);
	}
}
