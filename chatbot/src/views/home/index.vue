<script setup lang="ts">
import { useRouter } from 'vue-router';
import { SvgIcon } from '@/components/common';
import { ref } from 'vue';

// 状态管理
const username = ref('');
const password = ref('');
const errorMessage = ref('');
// const rightMessage = ref('');

// 路由管理
const router = useRouter();

// 登录逻辑
const handleLogin = () => {
  // 模拟简单的验证逻辑
  if (username.value === 'lili' && password.value === '123456') {
    // rightMessage.value = '登录成功！';
    router.replace({ name: 'Main' });
  } else {
    errorMessage.value = '用户名或密码错误！';
  }
};
</script>

<template>
  <div>
    <div id="login-form" class="flex flex-col items-center">
      <input type="text" v-model="username" placeholder="用户名" />
      <input type="password" v-model="password" placeholder="密码" />
      <div id="login-btn" @click="handleLogin">
        <!-- <p v-if="rightMessage" class="error-message">{{ rightMessage }}</p> -->
        <SvgIcon icon="ri-login-circle-line" />
      </div>
      <p v-if="errorMessage" class="error-message">{{ errorMessage }}</p>
    </div>
    <div class="message">
      基于LLM的个人智能助手系统
    </div>
  </div>
</template>

<style scoped>
.bg-image {
  background-image: url('./src/image/newweb.jpg');
  background-size: cover;
  background-position: center;
  background-repeat: no-repeat;
}

#login-form {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  text-align: center;
  width: 300px;
}

#login-btn {
  position: relative;
  width: 50px;
  height: 50px;
  background-color: #0b3a0d;
  color: white;
  cursor: pointer;
  font-size: 20px;
  border-radius: 50%;
  display: flex;
  justify-content: center;
  align-items: center;
  transition-duration: .5s;
  transform-origin: center;
  margin-top: 20px;
  svg {
    width: 50px;
    height: 50px;
    color: #fff;
  }
}
.error-message {
  color: rgb(240, 2, 2);
  margin-top: 24px;
}
input {
  margin-bottom: 10px;
  padding: 5px;
  width: 100%;
  box-sizing: border-box;
}

#login-btn:hover {
  transform: scale(1.25);
}

.message {
  position: absolute;
  bottom: 70%;
  width: 100%;
  text-align: center;
  color: white;
  font-size: 65px;
}

@media (prefers-color-scheme: dark) {
  body {
    background: #121212;
  }
}
</style>