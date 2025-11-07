<script setup lang="ts">
import Dialog from "primevue/dialog";
import { useModalsStore } from "@/stores/modals";
import DatePicker from "@/component/modals/ModelsViews/BillDatePicker.vue";

const modalsStore = useModalsStore();
</script>

<template>
  <Dialog
    class="dialog-box"
    :id="modalsStore.datePickerId"
    :visible="modalsStore.datePickerVisible"
    modal
    @update:visible="
      modalsStore.datePickerVisible = false;
      modalsStore.datePickerId = undefined;
    "
    >
    <template #header>
      <h2>Historical Date Picker</h2>
    </template>
    <DatePicker />
  </Dialog>
</template>

<style scoped>
.dialog-box {
  width: 100%;
  max-width: 1024px;
}

</style>
