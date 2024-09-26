<script setup lang='ts'>
interface Emit {
  (e: 'click'): void
}

const emit = defineEmits<Emit>()

function handleClick() {
  emit('click')
}
</script>

<template>
  <button
    class="flex items-center justify-center w-10 h-10 transition rounded-full hover:bg-neutral-100 dark:hover:bg-[#414755]"
    @click="handleClick">
    <slot />
  </button>
</template>

<style scoped>
.flex.items-center.justify-center:hover {
  background-color: rgba(255,255,255, .2);
}
</style>