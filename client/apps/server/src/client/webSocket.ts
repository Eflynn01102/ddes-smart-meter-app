import { Server, Socket } from "socket.io";
import { ref } from "vue";
import express from "express";
import { createServer } from "node:http";
import type { ClientToServerEvents, ServerToClientEvents, InterServerEvents, SocketData } from "@client/config/src/index";
import { z } from "zod";

const app = express();
export const server = createServer(app);
export const port = 3000;
const io = new Server<ClientToServerEvents,  ServerToClientEvents, InterServerEvents, SocketData>(server);

const socketServer = ref<Socket<ClientToServerEvents, ServerToClientEvents, InterServerEvents, SocketData>>()
const socketIds = ref<string>();


io.on("connection", (socket) => {
  console.log("a user connected");

  socketServer.value = socket;
  socketIds.value = socket.id;

  socket.emit("data",
    { 
      clientId: socket.id,
      data: 10 
    }
  ); // Send initial data

  socket.on("hello", () => {
    console.log("Received 'hello' from client:", socket.id);
  })

  socket.on("disconnect", () => {
    console.log("user disconnected");
  });
})

io.engine.on("connection_error", (err) => {
  console.log("Error code", err.code);     // the error code, for example 1
  console.log("Error message", err.message);
  console.log("Error Context", err.context);
})

app.post("/bill_data", (req, res) => {
  const data =  res.json()
  
})

function sendDataToAllClients(socket: Socket<ClientToServerEvents,  ServerToClientEvents, InterServerEvents, SocketData>, data: SocketData) {
  socket.emit("data", data);
  console.log("Sent data to client");
}

setInterval(() => {
  if (!socketServer.value) return;
  sendDataToAllClients(socketServer.value, {
    clientId: socketIds.value || "no-id",
    data: Math.floor(Math.random() * 100)
  })
}, 5000)

