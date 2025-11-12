import { createServer } from "node:http";
import express from "express";
import bodyParser from "body-parser";
import { APIAlterType, APIBillData } from "@/types";
import { WebSocket } from "@/client/webSocketClass";
import type { SocketMeter } from "@client/config/src";

const app = express();
export const server = createServer(app);
export const port = 3000;

const webSocketInstance = WebSocket.getInstance();
webSocketInstance.intializeWebSocket(server);

app.use(bodyParser.json());

app.get("/hello-world", (_req, res) => {
	console.log("Hello World endpoint hit");
	res.send("Hello World!");
});

app.post("/bill_data", (req, res) => {
	const data = APIBillData.safeParse(req.body);
	if (!data.success)
		return res.status(400).json({ error: "Invalid data format" });
	console.log("Received bill data:", data.data);
	webSocketInstance.sendDataToClient("bill_data", data.data);
	res.status(200).json({ status: "Data sent to clients" });
});

app.post("/alert", (req, res) => {
	const data = APIAlterType.safeParse(req.body);
	if (!data.success)
		return res.status(400).json({ error: "Invalid data format" });
	console.log("Received alert data:", data.data);
	webSocketInstance.sendDataToClient("alert", data.data);
	res.status(200).json({ status: "Alert sent to clients" });
});

export function sendCurrentUsageToClients(usage: SocketMeter) {
	webSocketInstance.sendDataToClient("current_usage", usage)
}
