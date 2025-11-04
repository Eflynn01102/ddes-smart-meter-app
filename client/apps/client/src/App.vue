<script setup lang="ts">
import { useSocketStore } from "./stores/socketio";
import { computed } from "vue";
import Header from "@/component/Header.vue";

const socketStore = useSocketStore();

const socketData = computed(() => socketStore.billData);
</script>

<template>
  <div class="main">
    <Header />
    <RouterView />
  </div>
</template>

<style scoped>
.main {
  display: flex;
  flex-direction: column;
  justify-content: center;
  width: 100%;
  height: 100%;
}
</style>
