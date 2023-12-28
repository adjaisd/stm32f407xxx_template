#include "flash.h"

// 参考: https://blog.csdn.net/XiaoXiaoPengBo/article/details/73742242

void FLASH_Init(void) {
    uint32_t addr = FLASH_SECTOR_ADDR_8;
    addr = FLASH_SECTOR_ADDR_3;
    addr = FLASH_SECTOR_ADDR_4;
    char buf[] = "This is a test flash program.";
    for (uint8_t i = 0; i < strlen(buf); i++) {
        FLASH_WriteByte(addr + i, buf[i]);
        char res[] = " \r\n";
        res[0] = FLASH_ReadByte(addr);
        USART1_Print(res, strlen(res));
    }
}

uint8_t FLASH_GetSectorNum(uint32_t addr) {
    uint8_t ret;
    if (addr < FLASH_SECTOR_ADDR_1) ret = 0;
    else if (addr < FLASH_SECTOR_ADDR_2) ret = 1;
    else if (addr < FLASH_SECTOR_ADDR_3) ret = 2;
    else if (addr < FLASH_SECTOR_ADDR_4) ret = 3;
    else if (addr < FLASH_SECTOR_ADDR_5) ret = 4;
    else if (addr < FLASH_SECTOR_ADDR_6) ret = 5;
    else if (addr < FLASH_SECTOR_ADDR_7) ret = 6;
    else if (addr < FLASH_SECTOR_ADDR_8) ret = 7;
    else if (addr < FLASH_SECTOR_ADDR_9) ret = 8;
    else if (addr < FLASH_SECTOR_ADDR_10)ret = 9;
    else if (addr < FLASH_SECTOR_ADDR_11)ret = 10;
    else ret = 11;
    return ret;
}

void FLASH_SectorErase(uint8_t sec_num) {
    while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY); // 等待BSY位被清除
    if (sec_num > 11) return;
    CLEAR_BIT(FLASH->ACR, FLASH_ACR_DCEN);      // 关闭数据缓存使能

    // 开启SER
    SET_BIT(FLASH->CR, FLASH_CR_SER);           // 激活扇区擦除

    // 配置PSIZE，并行位数，电压3.3V，配置为0x2
    MODIFY_REG(FLASH->CR, 0x3 << FLASH_CR_PSIZE_Pos, 0x2 << FLASH_CR_PSIZE_Pos);
    //FLASH->CR &= (~(0x3 << FLASH_CR_PSIZE_Pos));   // 清零
    //FLASH->CR |= (0x2 << FLASH_CR_PSIZE_Pos);

    // 设置SNB，扇区
    MODIFY_REG(FLASH->CR, 0xf << FLASH_CR_SNB_Pos, sec_num << FLASH_CR_SNB_Pos);
    //FLASH->CR &= (~(0xf << FLASH_CR_SNB_Pos));   // 将SNB的3位清零
    //FLASH->CR |= (sec_num << FLASH_CR_SNB_Pos);

    // 触发擦除操作
    SET_BIT(FLASH->CR, FLASH_CR_STRT);
    while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);

    // 关闭SER位
    CLEAR_BIT(FLASH->CR, FLASH_CR_SER_Msk);
    //FLASH->CR &= (~FLASH_CR_SER);
    CLEAR_BIT(FLASH->CR, 0xf << FLASH_CR_SNB_Pos);
    //FLASH->CR &= (~(0xf << FLASH_CR_SNB_Pos));
}

void FLASH_MassErase(void) {
    while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);
    SET_BIT(FLASH->CR, FLASH_CR_MER);   // MER位置1，针对所有用户扇区激活擦除操作
    SET_BIT(FLASH->CR, FLASH_CR_STRT);  // STRT位置1，触发擦除操作
    while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);
}

uint8_t FLASH_ReadByte(uint32_t addr) {
    return *(__IO uint8_t *) addr;
}

void FLASH_WriteByte(uint32_t addr, uint8_t byte) {
    if (addr < FLASH_BASE) return; // 地址非法
    FLASH_unlock();
    // 擦除addr所在扇区
    while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);
    if (addr < 0x1fff0000) {    // 主存储器区域需擦除扇区
        FLASH_unlock(); // 解锁
        CLEAR_BIT(FLASH->ACR, FLASH_ACR_DCEN);    // 关闭数据缓冲使能
        if (FLASH_ReadByte(addr) != 0xff) {        // 不为0xff表示已经被写过，需清零
            uint8_t sec_num = FLASH_GetSectorNum(addr);
            FLASH_SectorErase(sec_num);
        }
    }

    while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);
    SET_BIT(FLASH->CR, FLASH_CR_PG);    // CR的PG位置1，激活Flash编程
    //对所需的存储器地址（在主存储器块或OTP区域内）执行数据写入操作：
    //00 x8 编程 、01 x16 编程 、10 x32 编程、11 x64 编程
    MODIFY_REG(FLASH->CR, 0x3 << FLASH_CR_PSIZE_Pos, 0x0 << FLASH_CR_PSIZE_Pos);
    //FLASH->CR &= (~(0x3 << (FLASH_CR_PSIZE_Pos)));   // 将PSIZE的2位清零
    //FLASH->CR |= (0x0 << FLASH_CR_PSIZE_Pos);           // x8编程

    *(__IO uint8_t *) addr = byte;
    while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);     // 等待BSY位被清除
    CLEAR_BIT(FLASH->CR, FLASH_CR_PG);  // 关闭PG位
}

void FLASH_unlock(void) {
    if ((FLASH->CR & FLASH_CR_LOCK) != RESET) {
        /* Authorize the FLASH Registers access */
        WRITE_REG(FLASH->KEYR, 0x45670123);
        WRITE_REG(FLASH->KEYR, 0xCDEF89AB);
//        FLASH->KEYR = 0x45670123;
//        FLASH->KEYR = 0xCDEF89AB;
    }
}

void FLASH_lock(void) {
    FLASH->CR |= FLASH_CR_LOCK;
}
