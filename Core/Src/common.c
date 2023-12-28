#include "common.h"

int g_time = 0;
int g_rxtx_flag = 0;
int g_rx_len = 0;
int g_rx_head = 0;
int g_rx_tail = 0;
char g_rx_buf[BUF_MAX_LEN] = "";
char g_dma_rx_buf[BUF_MAX_LEN] = "";
char g_dma_tx_buf[BUF_MAX_LEN] = "";
uint8_t g_adc_rx_buf[11] = {0};

#if 0

void delay_us(uint32_t us) {
    uint32_t temp;
    // SysTick->LOAD = us * (168000000 / 1000000 - 1); // 时间加载
    SysTick->LOAD = us * (168000000 / 8000000 - 1); // 时间加载
    SysTick->VAL = 0x00;                            // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk |   // 系统时钟
                     SysTick_CTRL_ENABLE_Msk;       // 开始倒数
    do {
        temp = SysTick->CTRL; // 获取定时器的状态值
    } while (temp & 0x01 && !(temp & (1 << 16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
    SysTick->VAL = 0X00;
}

void delay_ms(uint32_t ms) {
    uint32_t temp;
    // SysTick->LOAD = ms * (168000000 / 1000 - 1);  // 时间加载
    SysTick->LOAD = ms * (168000000 / 8000 - 1);  // 时间加载
    SysTick->VAL = 0x00;                          // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | // 系统时钟
                     SysTick_CTRL_ENABLE_Msk;     // 开始倒数
    do {
        temp = SysTick->CTRL; // 获取定时器的状态值
    } while (temp & 0x01 && !(temp & (1 << 16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
    SysTick->VAL = 0X00;
}

#else

void delay_us(uint32_t us) {
    int i, j;
    for (i = 0; i < us; i++) {
        for (j = 0; j < 20; j++) {}
    }
}

void delay_ms(uint32_t ms) {
    delay_us(ms * 1000);
}

#endif

/*
gcc内联汇编参考:
1. https://blog.csdn.net/M0XFFF/article/details/122035027
2. blog.csdn.net/luteresa/article/details/119327138
armcc内联汇编参考: https://developer.arm.com/documentation/ka002724/latest/
 */
#if defined(__CC_ARM)
void MSR_MSP(uint32_t addr) {
    // set Main Stack value
    __asm {
        msr msp, addr
    }
    // __asm__("bx r14");   // 默认会补上"bx lr"
}
#else

void MSR_MSP(uint32_t addr) {
    // set Main Stack value
    __asm__ __volatile__(
            "msr msp, %[in];"
            :
            : [in]"r"(addr)
    );
    // __asm__("bx r14");   // 默认会补上"bx lr"
}

#endif

void iap_load_app(uint32_t appxaddr) {
    iapfun jump2app = (iapfun) *(uint32_t *) (appxaddr + 4); // 拷贝APP程序的复位中断函数地址
    MSR_MSP(*(uint32_t *) appxaddr); // 初始化APP堆栈指针,对APP程序的堆栈进行重构,就是说重新分配RAM
    jump2app();
}
