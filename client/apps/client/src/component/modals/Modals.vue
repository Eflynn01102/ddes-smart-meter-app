<script setup lang="ts">
import Dialog from "primevue/dialog";
import { useModalsStore } from "@/stores/modals";

const modalsStore = useModalsStore();

</script>

<template>
  <Dialog
    class="dialog-box"
    :id="modalsStore.signUpId"
    :visible="modalsStore.signUpVisible"
    modal
    @update:visible="
      modalsStore.signUpVisible = false;
      modalsStore.signUpId = undefined;
    "
    >
    <template #header>
      <h3>Modal Title</h3>
    </template>
    <p>This is the content of the modal.</p>
  </Dialog>
</template>

<style scoped>
.dialog-box {
  width: 100%;
  max-width: 1024px;
}

</style>
