import { Server } from "socket.io";
import express from "express";
import { createServer } from "node:http";
import type { ClientToServerEvents, ServerToClientEvents, InterServerEvents, SocketData } from "@client/config/src/index";

const app = express();
export const server = createServer(app);
export const port = 3000;
const io = new Server<ClientToServerEvents,  ServerToClientEvents, InterServerEvents, SocketData>(server);

app.get("/", (req, res) => {
  res.send({
    message: "hello world"
  });
  console.log("API endpoint hit");
})


io.on("connection", (socket) => {
  console.log("a user connected");

  socket.emit("data", 
    { data: 10 }
  ); // Send initial data

  socket.on("disconnect", () => {
    console.log("user disconnected");
  });
})
