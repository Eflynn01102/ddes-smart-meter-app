import { io, Socket } from "socket.io-client";
import { defineStore } from "pinia";
import { computed, ref } from "vue";
import type { ClientToServerEvents, ServerToClientEvents, SocketData } from "@client/config/src/index";

export const useSocketStore = defineStore("socketio", () => {

  const socketData = ref<SocketData>();

  const socket: Socket<ServerToClientEvents, ClientToServerEvents> = io("/");
  
  const isSocketActive = computed(() => socket.active);

  socket.on("connect", () => {
    console.log("Connected to server with ID:", socket.id);
  })

  socket.emit("hello");

  socket.on("data", (data: SocketData) => {
    console.log("Received data from server:", data);
    socketData.value = data;
  });

  socket.on("connect_error", (err) => {
    console.log(err.name)
    console.log(err.message);
  })


  return { socketData, isSocketActive };
})
