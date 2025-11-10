<script setup lang="ts">
import DatePicker from "primevue/datepicker";
import Button from "primevue/button";
import { ref } from "vue";

const date = ref<Date>();

// wireup to websocketStore for later dashboard

</script>

<template>
  <div class="modal-content">
    <span>Please enter a date below:</span>
    <DatePicker v-model="date" :invalid="!date" placeholder="Date" fluid />
    <Button label="Confirm" />
  </div>
</template>


<style scoped>
.modal-content {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}
</style>
