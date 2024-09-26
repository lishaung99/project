/*
 * @Description: 
 * @Author: lishuang
 * @Date: 2024-09-20 10:09:18
 * @FilePath: \chatboot\chatgpt-web\src\store\modules\user\helper.ts
 * @LastEditTime: 2024-09-20 15:56:26
 * @LastEditors: lishuang
 */
import { ss } from '@/utils/storage'

const LOCAL_NAME = 'userStorage'

export interface UserInfo {
  avatar: string
  name: string
  description: string
}

export interface UserState {
  userInfo: UserInfo
}

export function defaultSetting(): UserState {
  return {
    userInfo: {
      avatar: '@image/black.jpg',
      name: 'lili',
      description: 'Star on <a href="https://github.com/lishaung99" class="text-blue-500" target="_blank" >GitHub</a>',
    },
  }
}

export function getLocalState(): UserState {
  const localSetting: UserState | undefined = ss.get(LOCAL_NAME)
  return { ...defaultSetting(), ...localSetting }
}

export function setLocalState(setting: UserState): void {
  ss.set(LOCAL_NAME, setting)
}
