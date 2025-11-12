<script setup lang="ts">
import Knob from "primevue/knob";
import BarChart from "@/component/charts/BarChart.vue";
import { useSocketStore } from "@/stores/socketio";
import { computed } from "vue";

const socketStore = useSocketStore();

const socketData = computed(() => socketStore.billData);
</script>

<template>
  <div class="dashboard">
    <!-- Graphs -->
    <div class="graph" >

    </div>

    <!-- Bill -->
    <div class="bill">

    </div>

    <!-- Graphs -->
    <div class="graphs">

    </div>
  </div>
</template>

<style scoped>
.dashboard {
  width: 100%;
  height: 100%;
  display: flex;
  flex-direction: row;
  align-items: center;
}

.bill {
  width: 50%;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 1rem;
  box-shadow: 0 2px 4px var(--color-octo-blue);
  padding: 1rem;
}

.graphs {
  width: 25%;
}
</style>
