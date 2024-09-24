/*
 * @Description: 
 * @Author: lishuang
 * @Date: 2024-09-20 10:09:18
 * @FilePath: \chatboot\chatgpt-web\src\main.ts
 * @LastEditTime: 2024-09-20 14:41:54
 * @LastEditors: lishuang
 */
import { createApp } from 'vue'
import App from './App.vue'
import { setupI18n } from './locales'
import { setupAssets, setupScrollbarStyle } from './plugins'
import { setupStore } from './store'
import { setupRouter } from './router'
import './styles/global.less'

async function bootstrap() {
  const app = createApp(App)
  setupAssets()

  setupScrollbarStyle()

  setupStore(app)

  setupI18n(app)

  await setupRouter(app)

  app.mount('#app')
}

bootstrap()
