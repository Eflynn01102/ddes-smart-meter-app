<script setup lang="ts">
import { useSocketStore } from "@/stores/socketio";
import { computed } from "vue";

const socketStore = useSocketStore();

const socketData = computed(() => socketStore.billData);

</script>

<template>
  <div>
    <span>Bill Data from Socket.IO: {{ socketData }}</span>
  </div>
</template>

<style scoped>

</style>
