#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*
测试步骤:
1(控制led)、2(定时器控制led)、3(USART发送和接收)、4(USART+DMA发送和接收)
5(SPI发送和接收)、6(看USART波形图)、7(看SPI波形图)
8(flash读写)、9(中断向量表重定位)、10(boot程序用于IAP)、
11(以太网tcp和udp)
 */
#define TEST_STEP 11

/* IAP的APP程序起始位置，加入LWIP后bin文件分别需要64KB(采用gcc)和大约310KB(采用armcc5) */
#define APP_FLASH_ORIGIN (0x08040000)

/* 定时器控制led方法: 1(定时器)，2(定时器+全局变量) */
#define TIM_LED_METHOD 1

/* 是否从默认位置启动 */
#define USE_IAP 1

/* 缓冲区最大长度 */
#define BUF_MAX_LEN 50

#ifdef __cplusplus
}
#endif

#endif //__CONFIG_H__
