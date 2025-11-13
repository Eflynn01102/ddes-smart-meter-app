<script setup lang="ts">
import Toast from "primevue/toast";
import Header from "@/component/Header.vue";
import Modals from "@/component/modals/Modals.vue";

import router from "@/router";
import { onMounted } from "vue";
import { useAuthStore } from "@/stores/auth";

const authStore = useAuthStore();

onMounted(() => {
  if(!authStore.knownClientId) router.push({ path: "/" });
});

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
