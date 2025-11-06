<script setup lang="ts">
import Header from "@/component/Header.vue";
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
  align-items: center;
  justify-content: center;
  gap: 1rem;
  width: 100%;
  height: 100%;
}
</style>
