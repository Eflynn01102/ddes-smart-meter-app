import { defineStore } from "pinia";
import { ref } from "vue";

export const useModalsStore = defineStore("modals", () => {
	const signUpId = ref<string>();
	const signUpVisible = ref(false);

	return { signUpVisible, signUpId };
});
