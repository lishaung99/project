/*
 * @Description: 
 * @Author: lishuang
 * @Date: 2024-09-20 10:09:18
 * @FilePath: \chatboot\chatgpt-web\src\store\modules\settings\helper.ts
 * @LastEditTime: 2024-09-20 14:14:26
 * @LastEditors: lishuang
 */
import { ss } from '@/utils/storage'

const LOCAL_NAME = 'settingsStorage'

export interface SettingsState {
  systemMessage: string
  temperature: number
  top_p: number
}

export function defaultSetting(): SettingsState {
  return {
    systemMessage: 'You are Llama3, a large language model trained by Meta. Follow the user\'s instructions carefully. Respond using markdown.',
    temperature: 0.8,
    top_p: 1,
  }
}

export function getLocalState(): SettingsState {
  const localSetting: SettingsState | undefined = ss.get(LOCAL_NAME)
  return { ...defaultSetting(), ...localSetting }
}

export function setLocalState(setting: SettingsState): void {
  ss.set(LOCAL_NAME, setting)
}

export function removeLocalState() {
  ss.remove(LOCAL_NAME)
}
