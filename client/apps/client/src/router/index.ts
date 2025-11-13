import { createRouter, createWebHistory } from "vue-router";

import Login from "@/views/Login.vue";
import DashBoard from "@/views/DashBoard.vue";
import HistoricalDashBoard from "@/views/HistoricalDashBoard.vue";

const router = createRouter({
	history: createWebHistory(import.meta.env.BASE_URL),
	routes: [
		{
			path: "/",
			name: "login",
			component: Login,
		},
		{
			path: "/bill",
			name: "bill",
			component: DashBoard,
		},
		{
			path: "/history/:date",
			name: "history",
			component: HistoricalDashBoard,
		}
	],
});

export default router;
