import "./assets/main.css";

import { createApp } from "vue";
import PrimeVue from "primevue/config";
import { createPinia } from "pinia";

import lara from '@primeuix/themes/lara';

import App from "./App.vue";
import router from "./router";

const app = createApp(App, {
  theme: {
    preset: lara
  }
});

app.use(PrimeVue);

app.use(createPinia());
app.use(router);

app.mount("#app");
