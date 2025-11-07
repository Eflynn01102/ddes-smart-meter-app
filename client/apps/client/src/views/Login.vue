<script setup lang="ts">
import Button from "primevue/button";
import InputText from "primevue/inputtext";

import { useAuthStore } from "@/stores/auth";
import { useModalsStore } from "@/stores/modals";
import { ref } from "vue";

const modalsStore = useModalsStore();
const authStore = useAuthStore();

const username = ref("");
const password = ref("");

function login() {
	authStore.loginHandler(username.value, password.value);
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
          <InputText placeholder="Username" v-model=username :fluid="true" />
          <InputText placeholder="Password" type="password" v-model=password :fluid="true" style="margin-top: 1rem;" />
        </div>
        <br />
        <Button @click="login()" label="login" :fluid="true"/>
      </div>
      <div class="login-card-footer">
        <span>Don't have an account? </span>
        <a @click="modalsStore.signUpVisible = true; modalsStore.signUpId = 'signup-modal'">
          <span style="margin-left: 0.5rem; cursor: pointer; color: #60F0F8;"><u>Sign Up</u></span>
        </a>
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
  box-shadow: 0 2px 4px var(--color-octo-blue);
  padding: 1rem;
}
.login-card-body {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}
.login-card-footer {
  display: flex;
  flex-direction: row;
}
</style>
