import { io } from "socket.io-client";
import { defineStore } from "pinia";
import { ref } from "vue";

export const useSocketStore = defineStore("socketio", () => {
  const serverData = ref(0);

  const socket = io("/ws", { autoConnect: true });

  socket.on("connect", () => {
    console.log("Connected to server");
  })

  socket.emit("hello", "Hello from client");

  socket.on("data", (data) => {
    console.log("Data received from server:", data);
    serverData.value = data;
  });

  socket.on("connection_error", (err) => {
    console.log(err.description)
    console.log(err.message);  // the error message, for example "Session ID unknown"
    console.log(err.context);  // some additional error context
  })


  return { serverData };
})
