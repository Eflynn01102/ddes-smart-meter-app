<script setup lang="ts">
import { computed } from "vue";
import Chart from "primevue/chart";

const p = defineProps<{
  dataSet: number[];
  dataSet2: number[];
  label: string;
  label2: string;
  labels: string[];
}>();

const dataSet = computed(() => p.dataSet);
const dataSet2 = computed(() => p.dataSet2);

const config = { animation: 0 };

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
      },
      {
        label: p.label2,
        data: dataSet2.value,
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
  <div>
    <Chart type="bar" :data="chartData" :options="config" />
  </div>
</template>
