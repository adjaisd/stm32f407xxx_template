/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f4xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) {
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void) {
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void) {
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void) {
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void) {
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void) {
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void) {
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void) {
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void) {
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */

    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void) {
    /* USER CODE BEGIN EXTI0_IRQn 0 */

    /* USER CODE END EXTI0_IRQn 0 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET) {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
        /* USER CODE BEGIN LL_EXTI_LINE_0 */

        /* USER CODE END LL_EXTI_LINE_0 */
    }
    /* USER CODE BEGIN EXTI0_IRQn 1 */

    /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles SPI1 global interrupt.
  */
void SPI1_IRQHandler(void) {
    /* USER CODE BEGIN SPI1_IRQn 0 */
#if TEST_STEP == 5
    if (LL_SPI_IsActiveFlag_RXNE(SPI1)) {
        char rev_ch = LL_SPI_ReceiveData8(SPI1);
        if (g_rx_len >= BUF_MAX_LEN) // 去掉"\r\n"
        {
            g_rx_buf[g_rx_len - 3] = '\r';
            g_rx_buf[g_rx_len - 2] = '\n';
            g_rx_buf[g_rx_len - 1] = 0;
            g_rx_len = BUF_MAX_LEN - 1; // 不置为0，会在下次串口中断时判断IDLE标志位中置为0
            g_rxtx_flag = 1;            // stop receive
        }
        if (g_rxtx_flag == 0) // 继续接收
        {
            g_rx_buf[g_rx_len++] = LL_USART_ReceiveData8(USART1);
        }
    }
#elif TEST_STEP == 9
    if (LL_SPI_IsActiveFlag_TXE(SPI1)) {
    }
#endif
    /* USER CODE END SPI1_IRQn 0 */
    /* USER CODE BEGIN SPI1_IRQn 1 */

    /* USER CODE END SPI1_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void) {
    /* USER CODE BEGIN USART1_IRQn 0 */
#if TEST_STEP == 3
    // USART接收
    // ref: https://blog.csdn.net/junfoot/article/details/124313095
    // Rx非空中断，表明接收到了一个字符
    // 读取Rx可自动清除中断标志位
    if (LL_USART_IsActiveFlag_RXNE(USART1)) {
        // 如果数组长度大于设置的data最大长度，直接停止接收
        if (g_rx_len >= BUF_MAX_LEN) // 去掉"\r\n"
        {
            g_rx_buf[g_rx_len - 3] = '\r';
            g_rx_buf[g_rx_len - 2] = '\n';
            g_rx_buf[g_rx_len - 1] = 0;
            g_rx_len = BUF_MAX_LEN - 1; // 不置为0，会在下次串口中断时判断IDLE标志位中置为0
            g_rxtx_flag = 1;            // stop receive
            LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
        }
        if (g_rxtx_flag == 0) // 继续接收
        {
            g_rx_buf[g_rx_len++] = LL_USART_ReceiveData8(USART1);
        }
    }
    // 空闲中断，表明接收到1帧所有的数据
    if (LL_USART_IsActiveFlag_IDLE(USART1)) {
        // 清除中断标志
        LL_USART_ClearFlag_IDLE(USART1);
        g_rx_buf[g_rx_len] = 0;
        g_rx_len = 0;
        g_rxtx_flag = 1;
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    }

    /* USER CODE END USART1_IRQn 0 */
    /* USER CODE BEGIN USART1_IRQn 1 */
#elif TEST_STEP == 4
    // USART+DMA接收
    // ref: https://www.cnblogs.com/mftang2018/p/16617962.html
    if (LL_USART_IsActiveFlag_RXNE(USART1)) {
        LL_USART_ClearFlag_RXNE(USART1);
        LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
        LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_2, (uint32_t) g_dma_rx_buf);
        LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, BUF_MAX_LEN);
        LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
    }
    if (LL_USART_IsActiveFlag_IDLE(USART1)) {
        LL_USART_ClearFlag_IDLE(USART1); // 清除发送完成标志
        // 接收数据的长度LL_DMA_GetDataLength(DMA2, LL_DMA_STREAM_2);得到的是剩余的内存大小
        g_rx_head = g_rx_tail;
        g_rx_tail = BUF_MAX_LEN - LL_DMA_GetDataLength(DMA2, LL_DMA_STREAM_2);
        int c = 0, len = (g_rx_tail - g_rx_head + BUF_MAX_LEN) % BUF_MAX_LEN;
        for (uint8_t i = 0; i < len; i++) {
            int cur = (g_rx_head + i) % BUF_MAX_LEN;
            g_dma_tx_buf[c++] = g_dma_rx_buf[cur];
        }
        g_dma_tx_buf[c] = 0;
        g_rxtx_flag = 1;
    }
    LL_USART_ClearFlag_PE(USART1);
    LL_USART_ClearFlag_FE(USART1);
    LL_USART_ClearFlag_NE(USART1);
    LL_USART_ClearFlag_ORE(USART1);
    LL_USART_ClearFlag_IDLE(USART1);
#elif TEST_STEP == 5
    if (LL_USART_IsActiveFlag_RXNE(USART1)) {
        char rev_ch = LL_USART_ReceiveData8(USART1);
        if (g_rx_len >= BUF_MAX_LEN) // 去掉"\r\n"
        {
            g_rx_buf[g_rx_len - 3] = '\r';
            g_rx_buf[g_rx_len - 2] = '\n';
            g_rx_buf[g_rx_len - 1] = 0;
            g_rx_len = BUF_MAX_LEN - 1;
            g_rxtx_flag = 1;            // stop receive
        }
        if (g_rxtx_flag == 0) // 继续接收
        {
            g_rx_buf[g_rx_len++] = LL_USART_ReceiveData8(USART1);
        }
    }
    // 空闲中断，表明接收到1帧所有的数据
    if (LL_USART_IsActiveFlag_IDLE(USART1)) {
        // 清除中断标志
        LL_USART_ClearFlag_IDLE(USART1);
        g_rx_buf[g_rx_len] = 0;
        g_rx_len = 0;
        g_rxtx_flag = 1;
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    }
#elif TEST_STEP == 6
    // 测试
    if (LL_USART_IsActiveFlag_RXNE(USART1)) {
        g_rx_buf[0] = LL_USART_ReceiveData8(USART1);
        g_rx_buf[1] = 0;
        g_rxtx_flag = 1;
    }
#elif TEST_STEP == 8
    if (LL_USART_IsActiveFlag_IDLE(USART1)) {
        LL_USART_ClearFlag_IDLE(USART1);
    }
    if (LL_USART_IsActiveFlag_RXNE(USART1)) {
        LL_USART_ClearFlag_RXNE(USART1);
    }
#endif
    /* USER CODE END USART1_IRQn 0 */
    /* USER CODE BEGIN USART1_IRQn 1 */

    /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void) {
    /* USER CODE BEGIN TIM7_IRQn 0 */
#if TEST_STEP == 1
    // 延迟函数控制led
#elif TEST_STEP >= 2
    // 定时器控制led
#if TIM_LED_METHOD == 1
    // method 1: 定时器
#if TEST_STEP != 9
    if (LL_TIM_IsActiveFlag_UPDATE(TIM7)) {
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
        LL_TIM_ClearFlag_UPDATE(TIM7);
    }
#else
    if (LL_TIM_IsActiveFlag_UPDATE(TIM7)) {
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
        LL_TIM_ClearFlag_UPDATE(TIM7);
    }
#endif
#elif TIM_LED_METHOD == 2
    // method 2: 定时器+全局变量
    if (LL_TIM_IsActiveFlag_UPDATE(TIM7))
    {
        g_time++;
        LL_TIM_ClearFlag_UPDATE(TIM7);
    }
#endif

#endif
    /* USER CODE END TIM7_IRQn 0 */
    /* USER CODE BEGIN TIM7_IRQn 1 */

    /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void) {
    /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */
#if TEST_STEP == 4
    // RX, peripheral to memory
    if (LL_DMA_IsActiveFlag_TE2(DMA2)) {
        LL_DMA_ClearFlag_TE2(DMA2);
    }

    if (LL_DMA_IsActiveFlag_TC2(DMA2)) {
        LL_DMA_ClearFlag_TC2(DMA2);
    }

    if (LL_DMA_IsActiveFlag_HT2(DMA2)) {
        LL_DMA_ClearFlag_HT2(DMA2);
    }

    if (LL_DMA_IsActiveFlag_FE2(DMA2)) {
        LL_DMA_ClearFlag_FE2(DMA2);
    }
#endif
    /* USER CODE END DMA2_Stream2_IRQn 0 */

    /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

    /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void) {
    /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */
#if TEST_STEP == 4
    // USART+DMA发送
    // TXC, memory to peripheral
    if (LL_DMA_IsActiveFlag_TE7(DMA2)) {
        LL_DMA_ClearFlag_TE7(DMA2);
    }

    if (LL_DMA_IsActiveFlag_TC7(DMA2)) {
        LL_DMA_ClearFlag_TC7(DMA2);
    }

    if (LL_DMA_IsActiveFlag_HT7(DMA2)) {
        LL_DMA_ClearFlag_HT7(DMA2);
    }

    if (LL_DMA_IsActiveFlag_FE7(DMA2)) {
        LL_DMA_ClearFlag_FE7(DMA2);
    }
#endif
    /* USER CODE END DMA2_Stream7_IRQn 0 */

    /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

    /* USER CODE END DMA2_Stream7_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
