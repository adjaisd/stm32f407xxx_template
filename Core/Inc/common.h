/* 自定义通用头文件*/
#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
#include <stdint.h>
#include "config.h"

extern int g_time;                     /* 为定时器提供的全局变量定时器 */
extern int g_rxtx_flag;                /* 接收发送标志位: 0(接收)，1(发送) */
extern int g_rx_len;                   /* USART或SPI模式接收数据的长度 */
extern char g_rx_buf[BUF_MAX_LEN];     /* USART接收数据缓冲区 */
extern int g_rx_head;                  /* USART+DMA模式采用循环队列的队头部 */
extern int g_rx_tail;                  /* USART+DMA模式采用循环队列的队尾部 */
extern char g_dma_rx_buf[BUF_MAX_LEN]; /* USART+DMA模式接收数据的循环队列 */
extern char g_dma_tx_buf[BUF_MAX_LEN]; /* USART+DMA模式存放发送数据的数组 */
extern char g_spi_rx_buf[BUF_MAX_LEN]; /* spi存放接收数据的数组 */
extern uint8_t g_adc_rx_buf[11];       /* adc存放接收数据的数组 */

typedef void (*iapfun)(void);

void MSR_MSP(uint32_t addr);

void iap_load_app(uint32_t appxaddr);

// 延时, 参考: https://blog.csdn.net/hust_xu/article/details/47088365
void delay_us(uint32_t us);

void delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif
#endif /*__COMMON_H__ */
