<script setup lang="ts">
// import { useSocketStore } from "@/stores/socketio";
import { watch } from "vue";
import { io, Socket } from "socket.io-client";
import { ref } from "vue";
import type { ClientToServerEvents, ServerToClientEvents, SocketData } from "@client/config/src/index";

// const socketStore = useSocketStore();

type APIRes = {
  message: string;
}

const data = ref<APIRes>()

fetch("/api")
.then(async (res) => {
  apiConnect.value = true;
  data.value = await res.json() as APIRes;
})
.catch(err => {
  console.error("API Error:", err);
  apiConnect.value = false;
});

const socket: Socket<ServerToClientEvents, ClientToServerEvents> = io("/socket.io");

const apiConnect = ref<boolean>(false);

socket.on("connect", () => {
  console.log("Connected to server with ID:", socket.id);
});

socket.on("data", (data: SocketData) => {
  console.log("Received data from server:", data);
  // socketStore.updateServerData(data);
});

socket.on("connect_error", (err) => {
  console.log(err.name)
  console.log(err.message);  // the error message, for example "Session ID unknown"
})

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
