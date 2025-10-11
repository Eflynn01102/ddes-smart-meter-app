import { Server } from "socket.io";
import express from "express";
import { createServer } from "node:http";

const app = express();
export const server = createServer(app);
export const port = 3000;
const io = new Server(server);

app.get("/", (req, res) => {
  res.send({
    message: "hello world"
  });
  console.log("API endpoint hit");
})


io.on("connection", (socket) => {
  console.log("a user connected");

  socket.emit("data", 42); // Send initial data

  socket.on("disconnect", () => {
    console.log("user disconnected");
  });
})


io.on("hello", (data) => {
  console.log("Data event received:", data);
})