<script setup lang="ts">
<<<<<<< HEAD
import Toast from "primevue/toast";
import Header from "@/component/Header.vue";
import Modals from "@/component/modals/Modals.vue";
=======
import { useSocketStore } from "./stores/socketio";
import { computed } from "vue";

const socketStore = useSocketStore();

const socketData = computed(() => socketStore.billData);
>>>>>>> 46fd292 (update)
</script>

<template>
  <Toast position="bottom-left" gorup="bl" />
  <div class="main">
    <Header />
    <RouterView />
  </div>
  <Modals />
</template>

<style scoped>
.main {
  height: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 1rem;
}
</style>
