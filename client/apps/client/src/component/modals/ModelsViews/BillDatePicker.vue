<script setup lang="ts">
import DatePicker from "primevue/datepicker";
import Button from "primevue/button";
import { ref } from "vue";
import { useSocketStore } from "@/stores/socketio";

const socketStore = useSocketStore();

const date = ref<Date>();

function confirm () {
  if (date.value){
    socketStore.requestHistoricalBillData(date.value.toLocaleDateString("en-GB", {
      day: "2-digit",
      month: "2-digit",
      year: "2-digit",
    }));
  }
}

</script>

<template>
  <div class="modal-content">
    <span>Please enter a date below:</span>
    <DatePicker v-model="date" :invalid="!date" placeholder="Date" fluid />
    <div class="buttons">
      <Button label="Confirm" @click="confirm()" fluid />
      <Button label="Cancel" severity="danger" fluid />
    </div>
  </div>
</template>


<style scoped>
.modal-content {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.buttons {
  display: flex;
  flex-direction: row;
  gap: 1rem;
}
</style>
