<script setup lang="ts">
import { useSocketStore } from "@/stores/socketio";
import { watch } from "vue";

const socketStore = useSocketStore();

fetch("/api")
  .then(res => res.json())
  .then(data => {
    console.log("API Response:", data);
  })
  .catch(err => {
    console.error("API Error:", err);
  });

watch(() => socketStore.serverData, () => {
  console.log("Server Data Updated:", socketStore.serverData);
})
</script>

<template>
  <div>
    <h1>Socket.IO Client</h1>
    {{ socketStore.serverData }}
  </div>
 
</template>

<style scoped>
</style>
