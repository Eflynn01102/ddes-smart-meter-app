import { defineStore } from "pinia";
import { ref } from "vue";
import { Config } from "@/config/config";
import { useRouter } from "vue-router";
import { useToast } from "primevue/usetoast";

export const useAuthStore = defineStore("auth", () => {

  const router = useRouter();
  const toast = useToast();

  const isLoggedIn = ref(false)

  function loginHandler(username: string, password: string) {

    if (username === "admin" && password === "admin") {
      isLoggedIn.value = true
      router.push({ name: 'bill' });
    } else {
      isLoggedIn.value = false
      toast.add({
        severity:'error',
        summary: 'Login Failed',
        detail:'Invalid username or password',
        life: 3000
      });
    }
  }

  function logoutHandler() {
    isLoggedIn.value = false
    router.push({ name: 'login' });
  }

  return { isLoggedIn, loginHandler, logoutHandler };
})
