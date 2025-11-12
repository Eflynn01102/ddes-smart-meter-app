<script setup lang="ts">
import Knob from "primevue/knob";
import BarChart from "@/component/charts/BarChart.vue";
import { useSocketStore } from "@/stores/socketio";
import { computed, ref } from "vue";
import { version } from "../../package.json";

const socketStore = useSocketStore();

const socketData = computed(() => socketStore.billData);


const energyCost = computed(() => {
  return socketData.value?.data.amountDue || 0;
})

const currentUsage = computed(() => {
  return socketStore.currentUsage?.currentUsage || 0;
})

const date = ref(new Date().toLocaleDateString("en-GB", {
  day: "2-digit",
  month: "2-digit",
  year: "2-digit",
}));

</script>

<template>
  <div class="dashboard">
    <!-- Graphs -->
    <div class="sidebar" >
      <div class="graph-card">
        <span>Energy Cost:</span>
        <Knob v-model="energyCost" valueTemplate="£{value}" readonly/>
      </div>

      <div class="graph-card">
        <span>Tax:</span>
        <Knob v-model="energyCost" valueTemplate="£{value}" readonly/>
      </div>
    </div>

    <!-- Bill -->
    <div class="bill-card">
      <h2>Your Bill - {{ date }}</h2>
      <div class="bill">
        <ul>
          <li>Amount Due - £{{ socketData?.data.amountDue }}</li>
          <li>Energy Cost - £{{ socketData?.data.energyCost }}</li>
          <li>Currency - {{ socketData?.data.currency }}</li>
          <li>Standing Charge - {{ socketData?.data.standingCharge }}</li>
          <li>Start Period - {{ socketData?.data.periodStart }}</li>
          <li>Tax - {{ socketData?.data.tax }}</li>
        </ul>
      </div>
      <span>Software Version: {{ version }}</span>
    </div>

    <!-- Graphs -->
    <div class="sidebar">
      <div class="graph-card">
        <span style="font">Current Usage:</span>
        <Knob v-model="currentUsage" valueTemplate="{value} kwh" readonly />
      </div>

      <div class="graph-card">
        <span style="font">Amount Due:</span>
        <Knob v-model="currentUsage" valueTemplate="{value} kwh" readonly />
      </div>
    </div>
  </div>
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
