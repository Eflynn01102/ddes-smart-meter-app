<script setup lang="ts">
import { computed } from "vue";
import Chart from "primevue/chart";

const p = defineProps<{
  value: number;
  maxValue: number;
  labels: string[];
  label: string;
}>();

const config = { animation: 0 };

const chartData = computed(() => {
  const documentStyle = getComputedStyle(document.body);

  return {
    labels: p.labels,
    datasets: [
      {
        label: p.label,
        data: [p.value, p.maxValue],
        backgroundColor: [
          documentStyle.getPropertyValue("--p-green-400"),
          documentStyle.getPropertyValue("--p-gray-200")
        ]
      }
    ]
  };
});
</script>

<template>
  <div>
    <Chart type="doughnut" :data="chartData" :options="config" />
  </div>
</template>
