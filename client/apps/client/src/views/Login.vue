<script setup lang="ts">
import Card from "primevue/card";
import Button from "primevue/button";

import { useRouter } from "vue-router";

const router = useRouter();

</script>

<template>
  <div class="main">
    <Card>
      <template #title>
        Login to your account
      </template>
      <template #content>
        <div class="card-body">
          <span>Please login to your account</span>
          <Button @click="router.push('bill')" label="login"/>
        </div>
      </template>
      <template #footer>
        <span>Don't have an account? Sign up</span>
      </template>
    </Card>
  </div>
</template>

<style scoped>
.main {
  display: flex;
  flex-direction: column;
  align-items: center;
  height: 100%;
  width: 100%;
}
.card-body {
  height: 100%;
  width: 60%;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 1rem;
  height: 100%;
}
</style>
