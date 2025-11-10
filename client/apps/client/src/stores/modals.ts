import { defineStore } from "pinia";
import { ref } from "vue";

export const useModalsStore = defineStore("modals", () => {
	const datePickerId = ref<string>();
	const datePickerVisible = ref(false);

	return { datePickerVisible, datePickerId };
});
