/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "dma.h"
#include "lwip.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */
#if (TEST_STEP == 9) && (USE_IAP == 1)
    SCB->VTOR = APP_FLASH_ORIGIN;
#elif TEST_STEP == 10
    uint32_t addr = APP_FLASH_ORIGIN;
    iap_load_app(addr);
#endif
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM7_Init();
    MX_USART1_UART_Init();
    MX_SPI1_Init();
    MX_LWIP_Init();
    /* USER CODE BEGIN 2 */

#if TEST_STEP == 3
    char buf[] = "OK";
    USART1_Print(buf, strlen(buf));
#elif TEST_STEP == 4
    char buf[] = "OK";
    USART1_Print(buf, strlen(buf));
#elif TEST_STEP == 5
    // 初始化ads1255
    NVIC_DisableIRQ(EXTI0_IRQn);
    ADS1255_init();
    NVIC_EnableIRQ(EXTI0_IRQn); /* 开启GPIOB_0外部中断，用于连续接收ads1255数据 */
    delay_ms(1);
    uint8_t i = 0;
#elif TEST_STEP == 8
    char buf[] = "OK";
    USART1_Print(buf, strlen(buf));
    FLASH_Init();
#elif TEST_STEP == 9
    USART1_Print("OK\r\n", strlen("OK\r\n"));
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
#endif

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
#if TEST_STEP == 1
        // 控制led
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
        LL_mDelay(500);
#elif TEST_STEP == 2
        // 定时器控制LED
        LL_mDelay(3);
#if TIM_LED_METHOD == 1
        // method 1: 定时器
#elif TIM_LED_METHOD == 2
        // method 2: 定时器+全局变量
        // ref: https://blog.csdn.net/qq_39842609/article/details/125860802
        if (g_time == 1)
        {
          LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
          g_time = 0;
        }
#endif

#elif TEST_STEP == 3
        // USART接收和发送
        if (g_rxtx_flag == 1) {
            USART1_Print("receive(USART) : ", strlen("receive(USART) : "));
            USART1_Print(g_rx_buf, strlen(g_rx_buf));
            g_rxtx_flag = 0;
        }
#elif TEST_STEP == 4
        // USART+DMA接收和发送
        if (g_rxtx_flag == 1) {
            USART1_DMA_Send("receive(USART+DMA) : ", strlen("receive(USART+DMA) : "));
            LL_mDelay(1); // 防止发送的接收数据覆盖
            USART1_DMA_Send(g_dma_tx_buf, strlen(g_dma_tx_buf));
            g_rxtx_flag = 0;
        }
#elif TEST_STEP == 5
        // SPI发送和接受
        SPI1_WriteReadByte(ADS1255_CMD_WAKEUP);
        SPI1_WriteReadByte(ADS1255_CMD_RESET);
        delay_ms(1);
        uint8_t wreg_vals[] = {i, i + 1, i + 2, i + 3};
        ADS1255_WriteRegs(REG_STATUS,
                          wreg_vals,
                          sizeof(wreg_vals) / sizeof(wreg_vals[0]));
        SPI1_WriteReadByte(ADS1255_CMD_SYNC);
        i++;
        if (g_rxtx_flag == 1) {
            delay_ms(1); // 防止发送的接收数据覆盖
            g_rxtx_flag = 0;
        }
#elif TEST_STEP == 7
        SPI1_WriteReadByte(0x98);
        delay_ms(1);
#elif TEST_STEP == 9
        static uint8_t i = 0;
        char buf[2] = {i + '0', 0};
        i = (i >= 9) ? 0 : i + 1;
        USART1_Print(buf, strlen(buf));
        LL_mDelay(100);
#endif
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5) {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_RCC_HSI_SetCalibTrimming(16);
    LL_RCC_HSI_Enable();

    /* Wait till HSI is ready */
    while (LL_RCC_HSI_IsReady() != 1) {

    }
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_8, 168, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1) {

    }
    while (LL_PWR_IsActiveFlag_VOS() == 0) {
    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {

    }
    LL_SetSystemCoreClock(168000000);

    /* Update the time base */
    if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
