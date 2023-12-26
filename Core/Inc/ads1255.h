#ifndef __ADS1255_H__
#define __ADS1255_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "spi.h"

// ref: https://blog.csdn.net/m0_51695041/article/details/119897013
// ref: https://e2echina.ti.com/support/archived-groups/helpgroup/f/forum/23038/stm32-ads1255

// 参考ads1255手册第18页
#define ADS1255_RATE_30000_SPS 0xF0
#define ADS1255_RATE_15000_SPS 0xE0
#define ADS1255_RATE_7500_SPS 0xD0
#define ADS1255_RATE_3750_SPS 0xC0
#define ADS1255_RATE_2000_SPS 0xB0
#define ADS1255_RATE_1000_SPS 0xA1
#define ADS1255_RATE_500_SPS 0x92
#define ADS1255_RATE_100_SPS 0x82
#define ADS1255_RATE_60_SPS 0x72
#define ADS1255_RATE_50_SPS 0x63
#define ADS1255_RATE_30_SPS 0x53
#define ADS1255_RATE_25_SPS 0x43
#define ADS1255_RATE_15_SPS 0x33
#define ADS1255_RATE_10_SPS 0x20
#define ADS1255_RATE_5_SPS 0x13
#define ADS1255_RATE_2D5_SPS 0x03

/* Register Map: 参考手册第30页 */
#define REG_STATUS 0x00
#define REG_MUX 0x01
#define REG_ADCON 0x02
#define REG_DRATE 0x03
#define REG_IO 0x04
#define REG_OFC0 0x05
#define REG_OFC1 0x06
#define REG_OFC2 0x07
#define REG_FSC0 0x08
#define REG_FSC1 0x09
#define REG_FSC2 0x0a

/* DRDY */
#define ADS1255_DRDY LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_0)
#define SetADS1255_DRDY LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0)
#define ClrADS1255_DRDY LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0)
/* CS */
#define SetADS1255_CS LL_GPIO_SetOutputPin(GPIOE, LL_GPIO_PIN_8)
#define ClrADS1255_CS LL_GPIO_ResetOutputPin(GPIOE, LL_GPIO_PIN_8)

/* SYNC */
#define SetADS1255_SYNC1 LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6)
#define ClrADS1255_SYNC1 LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6)

/* CLK */
#define SetADS1255_CLK LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_Pin_3)
#define ClrADS1255_CLK LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_Pin_3)

/* Command */
#define ADS1255_CMD_WAKEUP 0x00   /* Completes SYNC and Exits Standby Mode */
#define ADS1255_CMD_RDATA 0x01    /* Read Data */
#define ADS1255_CMD_RDATAC 0x03   /* Read Data Continuously*/
#define ADS1255_CMD_SDATAC 0x0f   /* Stop Read Data Continuously */
#define ADS1255_CMD_RREG 0x10     /* Read From REG rrr */
#define ADS1255_CMD_WREG 0x50     /* WRITE To REG rrr */
#define ADS1255_CMD_SELFCAL 0xf0  /* Offset and Gain Self-Calibration */
#define ADS1255_CMD_SELFOCAL 0xf1 /* Offset Self-Calibration */
#define ADS1255_CMD_SELFGCAL 0xf2 /* Gain Self-Calibration */
#define ADS1255_CMD_SYSOCAL 0xf3  /* System Offset Calibration */
#define ADS1255_CMD_SYSGCAL 0xf4  /* System Gain Calibration */
#define ADS1255_CMD_SYNC 0xfc     /* Synchronize the A/D Conversion */
#define ADS1255_CMD_STANDBY 0xfd  /* Begin Standby Mode */
#define ADS1255_CMD_RESET 0xfe    /* Reset to Power-Up Values */
#define ADS1255_CMD_WAKEUP1 0xff  /* Completes SYNC and Exists Standby Mode */



void do_sync(void);

// 初始化ads1255
void ADS1255_init(void);

// 寄存器操作参考文档第34页

/* 向ads1255寄存器地址写入数据 */
void ADS1255_WriteRegs(uint8_t reg_addr, const uint8_t *wreg_vals, uint8_t reg_num);

/* 从ads1255寄存器地址读取数据 */
void ADS1255_ReadRegs(uint8_t reg_addr, uint8_t *reg_arr, uint8_t reg_num);

void ADS1255_Reset(void);

#ifdef __cplusplus
}
#endif
#endif /*__ADS1255_H__ */
