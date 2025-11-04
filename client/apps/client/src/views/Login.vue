<script setup lang="ts">
import Card from "primevue/card";
import Button from "primevue/button";

</script>

<template>
  <div>
    <Card class="body">
      <template #title>
        Welcome Back!
      </template>
      <template #content>
        Please login to your account
      </template>
      <template #footer>
        <Button @click="console.log('hello world')" label="login"/>
      </template>
    </Card>
  </div>
</template>

<style scoped>
.body {
  display: flex;
  flex-direction: row;
  justify-content: space-between;
  width: 100%;
}

</style>