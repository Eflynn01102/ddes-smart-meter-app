<script setup lang="ts">
import Knob from "primevue/knob";
import Skeleton from "primevue/skeleton";
import { computed, ref } from "vue";
import { version } from "../../package.json";
import { useAuthStore } from "@/stores/auth";
import { useSocketStore } from "@/stores/socketio";

const socketStore = useSocketStore();
const authStore = useAuthStore();

const socketData = computed(() => socketStore.billData);

const energyCost = computed(() => socketData.value?.amountDue || 0);
const tax = computed(() => {
	if (!socketData.value?.tax) return 0;
	return parseInt(socketData.value?.tax);
});

const currentUsage = computed(
	() => socketStore.currentUsage?.currentUsage || 0,
);
const perviousUsage = ref(0);
const clientUsage = computed(() => {
	if (socketStore.currentUsage?.clientId === authStore.knownClientId) {
		perviousUsage.value = socketStore.currentUsage?.currentUsage;
		return socketStore.currentUsage?.currentUsage;
	} else {
		return perviousUsage.value;
	}
});

const date = ref(
	new Date().toLocaleDateString("en-GB", {
		day: "2-digit",
		month: "2-digit",
		year: "2-digit",
	}),
);
</script>

<template>

  <template v-if="!socketData">
    <div class="dashboard">
      <div class="sidebar">

        <div class="graph-card" style="padding: 1rem;">
          <Skeleton width="10rem" class="mb-2"/>
          <Skeleton width="100px" height="100px" shape="circle" />
        </div>

        <div class="graph-card" style="padding: 1rem;">
          <Skeleton width="10rem" class="mb-2"/>
          <Skeleton width="100px" height="100px" shape="circle" />
        </div>

      </div>

      <div class="bill-card">
        <Skeleton width="20rem" class="mb-2" borderRadius="16px"/>
        <div class="bill" style="align-items: center; justify-content: center;">
          <ul>
            <li><Skeleton width="15rem" height="1.5rem" class="mb-2" borderRadius="8px"/></li>
            <li><Skeleton width="15rem" height="1.5rem" class="mb-2" borderRadius="8px"/></li>
            <li><Skeleton width="15rem" height="1.5rem" class="mb-2" borderRadius="8px"/></li>
            <li><Skeleton width="15rem" height="1.5rem" class="mb-2" borderRadius="8px"/></li>
            <li><Skeleton width="15rem" height="1.5rem" class="mb-2" borderRadius="8px"/></li>
            <li><Skeleton width="15rem" height="1.5rem" class="mb-2" borderRadius="8px"/></li>
          </ul>
        </div>
        <Skeleton width="10rem" height="1.5rem" class="mb-2" borderRadius="8px"/>
      </div>


      <div class="sidebar">
        <div class="graph-card" style="padding: 1rem;">
          <Skeleton width="10rem" class="mb-2"/>
          <Skeleton width="100px" height="100px" shape="circle" />
        </div>

        <div class="graph-card" style="padding: 1rem;">
          <Skeleton width="10rem" class="mb-2"/>
          <Skeleton width="100px" height="100px" shape="circle" />
        </div>
      </div>

    </div>
  </template>

  <template v-else>
    <div class="dashboard">
      <!-- Graphs -->
      <div class="sidebar" >
        <div class="graph-card">
          <span>Energy Cost:</span>
          <Knob v-model="energyCost" valueTemplate="£{value}" readonly/>
        </div>
        <div class="graph-card">
          <span>Tax:</span>
          <Knob v-model="tax" valueTemplate="£{value}" readonly/>
        </div>
      </div>
      <!-- Bill -->
      <div class="bill-card">
        <h2>Your Bill - {{ date }}</h2>
        <div class="bill">
          <ul>
            <li>Amount Due - £{{ socketData.amountDue }}</li>
            <li>Energy Cost - £{{ socketData.energyCost }}</li>
            <li>Standing Charge - £{{ socketData.standingCharge }}</li>
            <li>Tax - £{{ socketData.tax }}</li>
            <li>Currency - {{ socketData.currency }}</li>
            <li>Start Period - {{ socketData.periodStart }}</li>
          </ul>
        </div>
        <span>Software Version: {{ version }}</span>
      </div>
      <!-- Graphs -->
      <div class="sidebar">
        <template v-if="authStore.knownClientId === 'client-131'">
          <div class="graph-card">
            <span style="font">Current Usage:</span>
            <Knob v-model="currentUsage" valueTemplate="{value} kwh" readonly />
          </div>
        </template>
        <template v-else>
          <div class="graph-card">
            <span style="font">Current Usage:</span>
            <Knob v-model="clientUsage" valueTemplate="{value} kwh" readonly />
          </div>
        </template>
        <div class="graph-card">
          <span style="font">Amount Due:</span>
          <Knob v-model="socketData.amountDue" valueTemplate="£{value}" readonly />
        </div>
      </div>
    </div>
  </template>
</template>

<style scoped>
.dashboard {
  width: 100%;
  height: 100%;
  display: flex;
  flex-direction: row;
  justify-content: space-between;
  align-items: center;
}

.bill-card {
  width: 50%;
  height: 95%;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: space-between;
  gap: 1rem;
  box-shadow: 0 2px 4px 0 var(--color-octo-blue);
  padding: 1rem;
}

.bill {
  width: 60%;
  height: 80%;
  display: flex;
  flex-direction: column;
  justify-content: center;
  border: 2px solid var(--color-octo-white);
  padding: 2rem 2rem 1rem 2rem;
}

ul {
  display: flex;
  flex-direction: column;
  gap: 2rem;
}

li {
  font-size: 1.5rem;
}

.sidebar {
  width: 25%;
  height: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: space-between;
  padding: 10rem 0 15rem 0;
}

.graph-card {
  width: 90%;
  display: flex;
  flex-direction: row;
  gap: 2rem;
  align-items: center;
  justify-content: center;
  box-shadow: 0 2px 4px var(--color-octo-blue);
}

</style>
