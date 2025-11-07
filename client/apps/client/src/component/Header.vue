<script setup lang="ts">
import Menu from "primevue/menu";
import Button from "primevue/button";
import { ref, watch } from "vue";
import { useAuthStore } from "@/stores/auth";
import { useModalsStore } from "@/stores/modals";

const modalsStore = useModalsStore();

const authStore = useAuthStore();

const menu = ref();
const items = ref([
	{
		label: "Options",
		items: [
			{
				label: "Logout",
				icon: "pi pi-sign-out",
				command: () => {
					authStore.logoutHandler();
				},
			},
		],
	},
]);

watch(
	() => authStore.isLoggedIn,
	(newVal) => {
		if (newVal) {
			console.log("not logged in");
			items.value = [
				{
					label: "Options",
					items: [
						{
							label: "Bill Date",
							icon: "pi pi-calendar",
							command: () => {
								modalsStore.datePickerVisible = true;
								modalsStore.datePickerId = "bill-date-picker-modal";
							},
						},
						{
							label: "LogoutS",
							icon: "pi pi-sign-out",
							command: () => {
								authStore.logoutHandler();
							},
						},
					],
				},
			];
		} else {
			items.value = [
				{
					label: "Options",
					items: [
						{
							label: "Logout",
							icon: "pi pi-sign-out",
							command: () => {
								authStore.logoutHandler();
							},
						},
					],
				},
			];
		}
	},
);

function toggleMenu(event: Event) {
	menu.value.toggle(event);
}

const time = ref(
	new Date().toLocaleString("en-GB", {
		hour12: false,
		minute: "2-digit",
		hour: "2-digit",
		second: "2-digit",
	}),
);
setInterval(() => {
	time.value = new Date().toLocaleString("en-GB", {
		hour12: false,
		minute: "2-digit",
		hour: "2-digit",
		second: "2-digit",
	});
}, 1000);
</script>

<template>
  <div class="header">
    <span>{{ time }}</span>
     <h1><u>Smart Meter</u></h1>
    <div style="padding-left: 2rem;">
      <Button type="button" icon="pi pi-ellipsis-v" @click="toggleMenu($event)" aria-haspopup="true" aria-controls="overlay_menu" />
      <Menu ref="menu" :model="items" :popup="true" />
    </div>
  </div>
</template>


<style scoped>
.header {
  width: 100%;
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: space-between;
  box-shadow: 0 2px 4px var(--color-octo-blue);
  padding: 1rem 1rem;
}

span {
  font-size: 1.5rem;
}
</style>
