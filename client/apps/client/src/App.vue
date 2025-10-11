<script setup lang="ts">
import { useSocketStore } from "@/stores/socketio";
import { watch } from "vue";

const socketStore = useSocketStore();

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
