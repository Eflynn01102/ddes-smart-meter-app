<script setup lang="ts">
import { useSocketStore } from "./stores/socketio";
import { computed } from "vue";

const socketStore = useSocketStore();

const socketData = computed(() => socketStore.billData);
</script>

<template>
  <div>
    <h1>Socket.IO Client</h1>
    <br />
    <span> Socket Active - {{ socketStore.isSocketActive }}</span>
    <br />
    <span> Socket Data - {{ socketData }}</span>
  </div>

</template>

<style scoped>

</style>
