/* 自定义通用头文件*/
#ifndef __FLASH_H__
#define __FLASH_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "usart.h"

#ifndef FLASH_BASE
#define FLASH_BASE (0x08000000UL)
#endif

#define FLASH_SECTOR_ADDR_0  ((uint32_t)0x08000000)
#define FLASH_SECTOR_ADDR_1  ((uint32_t)0x08004000)
#define FLASH_SECTOR_ADDR_2  ((uint32_t)0x08008000)
#define FLASH_SECTOR_ADDR_3  ((uint32_t)0x0800C000)
#define FLASH_SECTOR_ADDR_4  ((uint32_t)0x08010000)
#define FLASH_SECTOR_ADDR_5  ((uint32_t)0x08020000)
#define FLASH_SECTOR_ADDR_6  ((uint32_t)0x08040000)
#define FLASH_SECTOR_ADDR_7  ((uint32_t)0x08060000)
#define FLASH_SECTOR_ADDR_8  ((uint32_t)0x08080000)
#define FLASH_SECTOR_ADDR_9  ((uint32_t)0x080A0000)
#define FLASH_SECTOR_ADDR_10 ((uint32_t)0x080C0000)
#define FLASH_SECTOR_ADDR_11 ((uint32_t)0x080E0000)

void FLASH_Init(void);

uint8_t FLASH_GetSectorNum(uint32_t addr);

void FLASH_SectorErase(uint8_t sec_num);

void FLASH_MassErase(void);

uint8_t FLASH_ReadByte(uint32_t addr);

void FLASH_WriteByte(uint32_t addr, uint8_t byte);

void FLASH_unlock(void);

void FLASH_lock(void);

#ifdef __cplusplus
}
#endif
#endif /*__FLASH_H__ */
