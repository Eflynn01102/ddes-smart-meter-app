<script setup lang="ts">
import { ref } from "vue";
import router from "@/router";
import Button from "primevue/button";
import DatePicker from "primevue/datepicker";
import { useToast } from "primevue/usetoast";
import { useSocketStore } from "@/stores/socketio";
import { useModalsStore } from "@/stores/modals";

const toast = useToast();
const modalsStore = useModalsStore();
const socketStore = useSocketStore();

const date = ref<Date>();

function confirm() {
	if (date.value) {
		socketStore.requestHistoricalBillData(
			date.value.toLocaleDateString("en-GB", {
				day: "2-digit",
				month: "2-digit",
				year: "2-digit",
			}),
		);
		modalsStore.datePickerVisible = false;
		toast.add({
			severity: "success",
			summary: "Success",
			detail: "Date selected",
			life: 3000,
		});
		router.push({
			name: "history",
			params: { date: date.value.toISOString() },
		});
	} else {
		toast.add({
			severity: "error",
			summary: "Error",
			detail: "Please select a date",
			life: 3000,
		});
	}
}

function reset() {
	modalsStore.datePickerVisible = false;
	toast.add({
		severity: "success",
		summary: "Success",
		detail: "Bill Date reset to present",
		life: 3000,
	});
	router.push({ name: "bill" });
}
</script>

<template>
  <div class="modal-content">
    <span>Please enter a date below:</span>
    <DatePicker v-model="date" :invalid="!date" placeholder="Date" fluid />
    <div class="buttons">
      <Button label="Confirm" @click="confirm()" fluid />
      <Button label="Reset" @click="reset()" severity="danger" fluid />
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
