import { io } from "socket.io-client";
import { defineStore } from "pinia";
import { ref } from "vue";

export const useSocketStore = defineStore("socketio", () => {
  const serverData = ref(0);

  const socket = io("/ws");

  socket.on("connect", () => {
    console.log("Connected to server");
  })


  return { serverData };
})