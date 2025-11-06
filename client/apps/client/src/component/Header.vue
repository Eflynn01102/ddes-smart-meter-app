<script setup lang="ts">
import Menu from "primevue/menu";
import Button from "primevue/button";
import { ref, watch } from "vue";
import router from "@/router";
import { useAuthStore } from "@/stores/auth";
import { useToast } from "primevue/usetoast";

const toast = useToast()

const authStore = useAuthStore()

const menu = ref()
const items = ref([
  {
    label: 'Options',
    items: [
      {
        label: "Logout",
        icon: "pi pi-sign-out",
        command: () => {
          authStore.isLoggedIn = false
          router.push('/')
        }
      }
    ]
  }
])

watch(() => authStore.isLoggedIn, (newVal) => {
  if (newVal) {
    console.log("not logged in")
    items.value = [
      {
        label: 'Options',
        items: [
           {
            label: "Bill Date",
            icon: "pi pi-calendar",
            command: () => {
              toast.add({severity:'info', summary: 'Bill Date', detail: 'Your next bill date is 25th June 2024', life: 3000});
              console.log("Bill Date Clicked")
            }
          },
          {
            label: "Login",
            icon: "pi pi-sign-out",
            command: () => {
              router.push({ name: 'login'})
              authStore.isLoggedIn = false
            }
          }
        ]
      }
    ]
  } else {
    items.value = [
      {
        label: 'Options',
        items: [
          {
            label: "Logout",
            icon: "pi pi-sign-out",
            command: () => {
              authStore.isLoggedIn = false
              router.push({ name: 'login'})
            }
          }
        ]
      }
    ]
  }
})

function toggleMenu(event: Event) {
  menu.value.toggle(event)
}

const time = ref(new Date().toLocaleTimeString());
setInterval(() => {
  time.value = new Date().toLocaleTimeString();
}, 1000);

</script>

<template>
  <div class="header">
    <span>{{ time }}</span>
     <h1><u>Smart Meter</u></h1>
    <div>
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
  box-shadow: 0 2px 4px #60F0F8;
  padding: 1rem 1rem;
}

span {
  font-size: 1.5rem;
}
</style>
