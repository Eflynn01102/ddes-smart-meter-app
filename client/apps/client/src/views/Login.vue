<script setup lang="ts">
import Button from "primevue/button";

import { useAuthStore } from "@/stores/auth";
import { useRouter } from "vue-router";

const router = useRouter();
const authStore = useAuthStore();

function login() {
  authStore.isLoggedIn = true;
  router.push('bill');
}

</script>

<template>
  <div class="login-card">
    <div>
      <h2>Login to your account</h2>
    </div>
    <div>
      <span>Please login to your account</span>

      <br />
      <Button @click="login()" label="login" :fluid="true"/>
    </div>
    <div>
      <span>Don't have an account? Sign up</span>
    </div>
  </div>
</template>

<style scoped>
.login-card {
  height: 100%;
  width: 25%;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 1rem;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  padding: 1rem, 1rem;
}
</style>
