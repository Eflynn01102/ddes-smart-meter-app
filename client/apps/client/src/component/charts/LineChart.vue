<script setup lang="ts">
import {
  Chart as ChartJS,
  Title,
  Tooltip,
  Legend,
  LineElement,
  CategoryScale,
  LinearScale,
  type ChartData,
  PointElement
} from "chart.js";
import { computed, ref } from "vue";
import { Line } from "vue-chartjs";

const p = defineProps<{
  dataset1: number[];
  dataset2?: number[];
  dataset3?: number[];
  labels: string[];
  label1: string;
  label2?: string;
  label3?: string;
}>();

ChartJS.register(
  CategoryScale,
  LinearScale,
  LineElement,
  Title,
  Tooltip,
  Legend,
  PointElement
);

const chartData = computed<ChartData<"line">>(() => {
  if (!p.dataset2 && !p.dataset3) {
    return {
      labels: p.labels.map((label) => label),
      datasets: [
        {
          label: p.label1,
          data: p.dataset1.map((data) => data) ?? [],
          fill: false,
          borderColor: "rgb(0, 128, 0)",
          tension: 0.4,
          pointRadius: 0
        }
      ]
    };
  }
  if (!p.dataset3) {
    return {
      labels: p.labels.map((label) => label),
      datasets: [
        {
          label: p.label1,
          data: p.dataset1.map((data) => data) ?? [],
          fill: false,
          borderColor: "rgb(0, 128, 0)",
          tension: 0.4,
          pointRadius: 0
        },
        {
          label: p.label2,
          data: p.dataset2?.map((data) => data) ?? [],
          fill: false,
          borderColor: "rgb(255, 165, 0)",
          tension: 0.4,
          pointRadius: 0
        }
      ]
    };
  }

  if (!p.dataset3 && p.dataset1 && p.dataset3) {
    return {
      labels: p.labels.map((label) => label),
      datasets: [
        {
          label: p.label1,
          data: p.dataset1.map((data) => data) ?? [],
          fill: false,
          borderColor: "rgb(0, 128, 0)",
          tension: 0.4,
          pointRadius: 0
        },
        {
          label: p.label2,
          data: p.dataset2?.map((data) => data) ?? [],
          fill: false,
          borderColor: "rgb(0, 0, 255)",
          tension: 0.4,
          pointRadius: 0
        }
      ]
    };
  }

  if (!p.dataset2 && p.dataset1 && p.dataset3) {
    return {
      labels: p.labels.map((label) => label),
      datasets: [
        {
          label: p.label1,
          data: p.dataset1.map((data) => data) ?? [],
          fill: false,
          borderColor: "rgb(0, 128, 0)",
          tension: 0.4,
          pointRadius: 0
        },
        {
          label: p.label3,
          data: p.dataset3?.map((data) => data) ?? [],
          fill: false,
          borderColor: "rgb(0, 0, 255)",
          tension: 0.4,
          pointRadius: 0
        }
      ]
    };
  }

  return {
    labels: p.labels.map((label) => label),
    datasets: [
      {
        label: p.label1,
        data: p.dataset1.map((data) => data) ?? [],
        fill: false,
        borderColor: "rgb(0, 128, 0)",
        tension: 0,
        pointRadius: 0
      },
      {
        label: p.label2,
        data: p.dataset2?.map((data) => data) ?? [],
        fill: false,
        borderColor: "rgb(255, 165, 0)",
        tension: 0,
        pointRadius: 0
      },
      {
        label: p.label3,
        data: p.dataset3?.map((data) => data) ?? [],
        fill: false,
        borderColor: "rgb(0, 0, 255)",
        tension: 0,
        pointRadius: 0
      }
    ]
  };
});

const options = ref({ responsive: true });
</script>

<template>
  <div>
    <Line :data="chartData" :options="options" />
  </div>
</template>
