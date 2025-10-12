<script setup lang="ts">
// import { useSocketStore } from "@/stores/socketio";
import { watch } from "vue";
import { io, Socket } from "socket.io-client";
import { ref } from "vue";
import type { ClientToServerEvents, ServerToClientEvents, InterServerEvents, SocketData } from "@client/config/src/index";

// const socketStore = useSocketStore();

const socket: Socket<ServerToClientEvents, ClientToServerEvents> = io({
  transports: ["websocket"],
  autoConnect: true,
  reconnection: true,
  hostname: "localhost",
  port: 3000,
});

const apiConnect = ref<boolean>(false);

socket.on("connect", () => {
  console.log("Connected to server with ID:", socket.id);
});

socket.on("data", (data: SocketData) => {
  console.log("Received data from server:", data);
  // socketStore.updateServerData(data);
});

type APIRes = {
  message: string;
}

const data = ref<APIRes>()

const API = await fetch("/api").catch(err => {
    console.error("API Error:", err);
    apiConnect.value = false;
  });

// watch(() => socketStore.serverData, () => {
//   console.log("Server Data Updated:", socketStore.serverData);
// })
</script>

<template>
  <div>
    <h1>Socket.IO Client</h1>
    <span> API Hit - {{ apiConnect }}</span>
    <br />
    <span> Socket - {{ data?.message }}</span>
  </div>
 
</template>

<style scoped>
</style>
