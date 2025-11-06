<script setup lang="ts">
import Button from "primevue/button";
import InputText from "primevue/inputtext";

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
  <div class="login-main">
    <div class="login-card">
      <div>
        <h2>Login</h2>
      </div>
      <div>
        <span>Please login to your account</span>
        <br />
        <div>
          <InputText placeholder="Username" :fluid="true" />
          <InputText placeholder="Password" type="password" :fluid="true" style="margin-top: 1rem;" />
        </div>
        <br />
        <Button @click="login()" label="login" :fluid="true"/>
      </div>
      <div>
        <span>Don't have an account? Sign up</span>
      </div>
    </div>
  </div>
</template>

<style scoped>
.login-main {
  height: 100%;
  width: 25%;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}
.login-card {
  width: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 1rem;
  box-shadow: 0 2px 4px #60F0F8;
  padding: 1rem;
}
.login-card-body {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}
</style>
