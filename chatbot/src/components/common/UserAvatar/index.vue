<!--
 * @Description: 
 * @Author: lishuang
 * @Date: 2024-09-20 10:09:18
 * @FilePath: \chatboot\chatgpt-web\src\components\common\UserAvatar\index.vue
 * @LastEditTime: 2024-09-20 11:30:19
 * @LastEditors: lishuang
-->
<script setup lang='ts'>
import { computed } from 'vue'
import { NAvatar } from 'naive-ui'
import { useUserStore } from '@/store'
import defaultAvatar from '@/assets/my.jpg'
import { isString } from '@/utils/is'

const userStore = useUserStore()

const userInfo = computed(() => userStore.userInfo)
</script>

<template>
  <div class="flex items-center overflow-hidden">
    <div class="w-10 h-10 overflow-hidden rounded-full shrink-0">
      <template v-if="isString(userInfo.avatar) && userInfo.avatar.length > 0">
        <NAvatar
          size="large"
          round
          :src="userInfo.avatar"
          :fallback-src="defaultAvatar"
        />
      </template>
      <template v-else>
        <NAvatar size="large" round :src="defaultAvatar" />
      </template>
    </div>
    <div class="flex-1 min-w-0 ml-2" >
      <h2 class="overflow-hidden font-bold text-white text-ellipsis whitespace-nowrap">
        {{ userInfo.name ?? 'lili' }}
      </h2>
      <!-- <p class="overflow-hidden text-white text-gray-500 text-ellipsis whitespace-nowrap">
        <span
          v-if="isString(userInfo.description) && userInfo.description !== ''"
          v-html="userInfo.description"
        />
      </p> -->
    </div>
  </div>
</template>
