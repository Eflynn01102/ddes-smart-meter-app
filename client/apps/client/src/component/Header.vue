<script setup lang="ts">
import Menu from "primevue/menu";
import Button from "primevue/button";
import { computed, ref } from "vue";

const menu = ref()
const items = ref([
  {
    label: 'Options',
    items: [
      {
        label: "Logout",
        icon: "pi pi-sign-out",
      }
    ]
  }
])

function toggleMenu(event: Event) {
  menu.value.toggle(event)
}

const time = computed(() => {
  return new Date().toLocaleTimeString()
})

</script>

<template>
  <div class="header">
    <span>{{ time }}</span>
    <h1>Smart Meter</h1>
    <div>
      <Button type="button" icon="pi pi-ellipsis-v" @click="toggleMenu($event)" aria-haspopup="true" aria-controls="overlay_menu" />
      <Menu ref="menu" :model="items" :popup="true" />
    </div>
  </div>
</template>


<style scoped>
.header {
  width: 100%;
  height: 100%;
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: space-between;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  padding: 1rem 1rem;
}

span {
  font-size: 1.5rem;
  color: #666;
}
</style>
