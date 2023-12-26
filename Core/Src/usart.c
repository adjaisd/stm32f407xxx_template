/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* USART1 init function */

void MX_USART1_UART_Init(void) {

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    LL_USART_InitTypeDef USART_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    /**USART1 GPIO Configuration
    PA9   ------> USART1_TX
    PA10   ------> USART1_RX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */

    /* USART1_RX Init */
    LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_2, LL_DMA_CHANNEL_4);

    LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_2, LL_DMA_PRIORITY_HIGH);

    LL_DMA_SetMode(DMA2, LL_DMA_STREAM_2, LL_DMA_MODE_NORMAL);

    LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_2, LL_DMA_PERIPH_NOINCREMENT);

    LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_2, LL_DMA_MEMORY_INCREMENT);

    LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_2, LL_DMA_PDATAALIGN_BYTE);

    LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_2, LL_DMA_MDATAALIGN_BYTE);

    LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_2);

    /* USART1_TX Init */
    LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_7, LL_DMA_CHANNEL_4);

    LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_7, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

    LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_7, LL_DMA_PRIORITY_HIGH);

    LL_DMA_SetMode(DMA2, LL_DMA_STREAM_7, LL_DMA_MODE_NORMAL);

    LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_7, LL_DMA_PERIPH_NOINCREMENT);

    LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_7, LL_DMA_MEMORY_INCREMENT);

    LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_7, LL_DMA_PDATAALIGN_BYTE);

    LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_7, LL_DMA_MDATAALIGN_BYTE);

    LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_7);

    /* USART1 interrupt Init */
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(USART1_IRQn);

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART1, &USART_InitStruct);
    LL_USART_ConfigAsyncMode(USART1);
    LL_USART_Enable(USART1);
    /* USER CODE BEGIN USART1_Init 2 */
#if TEST_STEP == 3
    LL_USART_EnableIT_IDLE(USART1); // 串口空闲使能
    LL_USART_EnableIT_RXNE(USART1); // 串口接收非空使能
#elif TEST_STEP == 4
    // 配置串口方向，我这里是发送和接收都要
    LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);

    /* 接收配置 */
    // 设置外设地址，也就是设置串口的数据寄存器
    LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_2, (uint32_t) &USART1->DR);
    // 设置内存地址，也就是设置接收的数据要放哪
    LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_2, (uint32_t) g_dma_rx_buf);
    // 设置接收最大数据长度
    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, BUF_MAX_LEN);
    // 使能DMA2发送完成中断
    LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);
    // 使能接收的DMA2 STREAM_2
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
    // 使能DMA串口接收
    LL_USART_EnableDMAReq_RX(USART1);

    /* 发送配置 */
    // 设置外设地址，因为这个是DMA串口发送
    LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_7, (uint32_t) &USART1->DR);
    // 使能DMA2发送完成中断
    LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_7);
    // 使能DMA串口发送
    LL_USART_EnableDMAReq_TX(USART1);

    LL_USART_EnableIT_IDLE(USART1); // 串口空闲使能
    LL_USART_EnableIT_RXNE(USART1); // 串口接收非空使能
#elif TEST_STEP == 5
    LL_USART_Disable(USART1);
#elif TEST_STEP == 6
    LL_USART_EnableIT_IDLE(USART1);
    LL_USART_EnableIT_RXNE(USART1);
#elif TEST_STEP == 7
    LL_USART_Disable(USART1);
#elif TEST_STEP == 8
    LL_USART_EnableIT_IDLE(USART1);
    LL_USART_EnableIT_RXNE(USART1);
#elif TEST_STEP == 9
    LL_USART_DisableIT_IDLE(USART1);
    LL_USART_DisableIT_RXNE(USART1);
#elif TEST_STEP == 10
    LL_USART_Disable(USART1);
#endif
    /* USER CODE END USART1_Init 2 */

}

/* USER CODE BEGIN 1 */
void USART1_Print(const char *text, uint32_t size) {
    for (uint32_t index = 0; index < size; index++) {
        // USART数据发送有两个标志，一个是TXE=1,发送缓冲区空标志：说明可以向数据寄存器写入数据了，但并不代码数据发送完成了
        while (!LL_USART_IsActiveFlag_TXE(USART1));
        LL_USART_TransmitData8(USART1, text[index]);
    }
    // 另一个是TC=1发送结束
    while (!LL_USART_IsActiveFlag_TC(USART1));
}

void USART1_DMA_Send(const char *str, uint32_t size) {
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_7);                   // 关闭DMA2 Stream7
    LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_7, (uint32_t) str); // 配置内存地址
    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_7, size);             // 配置数据长度
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_7);                    // 使能DMA2 STREAM7
}

/* USER CODE END 1 */
