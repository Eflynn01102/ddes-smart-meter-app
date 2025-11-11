<script setup lang="ts">
import { ref, computed } from "vue";
import Chart from "primevue/chart";

const p = defineProps<{ dataSet: number[]; labels: string[]; label: string }>();
const dataSet = computed(() => p.dataSet);

const config = ref({ animation: 0 });

const chartData = computed(() => {
  return {
    labels: p.labels,
    datasets: [
      {
        label: p.label,
        data: dataSet.value,
        backgroundColor: [
          "rgba(255, 0, 0, 0.2)",
          "rgba(0, 0, 255, 0.2)",
          "rgb(0, 255, 0, 0.2)",
          "rgba(255, 165, 0, 0.2)"
        ],
        borderColor: [
          "rgba(255, 0, 0, 0.2)",
          "rgba(0, 0, 255, 0.2)",
          "rgb(0, 255, 0, 0.2)",
          "rgba(255, 165, 0, 0.2)"
        ],
        borderWidth: 1
      }
    ]
  };
});
</script>

<template>
  <div class="card">
    <Chart type="bar" :data="chartData" :options="config" />
  </div>
</template>
