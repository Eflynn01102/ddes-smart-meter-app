import "./assets/main.css";

import { createApp } from "vue";
import PrimeVue from "primevue/config";
import { createPinia } from "pinia";

import ToastService from "primevue/toastservice";

import App from "./App.vue";
import router from "./router";
import { octoPreset } from "@/themes/theme";

const app = createApp(App);

app.use(PrimeVue, {
	theme: {
		preset: octoPreset,
	},
});

app.use(createPinia());
app.use(router);
app.use(ToastService);

app.mount("#app");
