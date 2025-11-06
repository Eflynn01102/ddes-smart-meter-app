<script setup lang="ts">
import { useSocketStore } from "@/stores/socketio";
import { computed } from "vue";

const socketStore = useSocketStore();

const socketData = computed(() => socketStore.billData);

</script>

<template>
  <div class="dashboard">
    <span>Bill Data from Socket.IO: {{ socketData }}</span>
  </div>
</template>

<style scoped>
.dashboard {
  width: 100%;
  height: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: space-between;
  padding: 1rem 1rem;
}
</style>
