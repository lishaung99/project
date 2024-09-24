<script setup lang='ts'>
import { defineAsyncComponent, ref } from 'vue'
import { HoverButton, SvgIcon, UserAvatar } from '@/components/common'
import { useRouter } from 'vue-router';

const Setting = defineAsyncComponent(() => import('@/components/common/Setting/index.vue'))

const show = ref(false)
const router = useRouter();
// 退出到主界面的逻辑
const handleExit = () => {
  router.push({ name: 'Home' });
};

</script>


<template>
  <footer
    class="flex items-center justify-between min-w-0 p-4 overflow-hidden border-t dark:border-neutral-800 chat-silder-footer">
    <div class="flex-1 flex-shrink-0 overflow-hidden">
      <UserAvatar />
    </div>

    <HoverButton @click="show = true">
      <span class="text-xl text-white custome-setting">
        <SvgIcon icon="ri:settings-4-line" />
      </span>
    </HoverButton>
    <Setting v-if="show" v-model:visible="show" />
    
    <HoverButton @click="handleExit">
      <span class="text-xl text-white custome-setting">
        <SvgIcon icon="ri:logout-circle-r-line" />
      </span>
    </HoverButton>
  </footer>
</template>

<style scoped>
.custome-setting {
  transition-duration: 1s;
}

.flex.items-center.justify-center:hover  .custome-setting {
  transform: rotateZ(180deg);
}

.text-white {
  color: rgb(240, 236, 236);
}
</style>

