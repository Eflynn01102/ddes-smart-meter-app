<script setup lang="ts">
import Toast from "primevue/toast";
import Header from "@/component/Header.vue";
</script>

<template>
  <Toast position="bottom-left" gorup="bl" />
  <div class="main">
    <Header />
    <RouterView />
  </div>
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
