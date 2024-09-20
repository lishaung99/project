<!--
 * @Description: 
 * @Author: lishuang
 * @Date: 2024-09-20 10:09:18
 * @FilePath: \chatboot\chatgpt-web\src\App.vue
 * @LastEditTime: 2024-09-20 15:40:21
 * @LastEditors: lishuang
-->
<script setup lang="ts">
import { NConfigProvider } from 'naive-ui'
import { NaiveProvider } from '@/components/common'
import { useTheme } from '@/hooks/useTheme'
import { useLanguage } from '@/hooks/useLanguage'

const { theme, themeOverrides } = useTheme()
const { language } = useLanguage()
</script>

<template>
  <NConfigProvider
    class="h-full background-image"
    :theme="theme"
    :theme-overrides="themeOverrides"
    :locale="language"
  >
    <NaiveProvider>
      <RouterView />
    </NaiveProvider>
  </NConfigProvider>
</template>
