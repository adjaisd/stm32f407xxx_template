#include "ads1255.h"

void do_sync(void) {
    ClrADS1255_SYNC1;
    delay_us(10);
    SetADS1255_SYNC1;
    while (!ADS1255_DRDY);
}

void ADS1255_init(void) {
    SetADS1255_SYNC1; // 同步

    // reset
    ClrADS1255_CS; // 片选ADS1255
    delay_us(10);
    SPI1_WriteReadByte(ADS1255_CMD_RESET);
    delay_us(10);
    SetADS1255_CS;

    // init
    ClrADS1255_CS;
    delay_us(10);
    const uint8_t wreg_vals[4] = {0x04, 0x01, 0x01, 0x82};
    uint8_t reg_num = sizeof(wreg_vals) / sizeof(wreg_vals[0]);
    do {
        delay_us(10);
        // write regs
        ADS1255_WriteRegs(REG_STATUS, wreg_vals, reg_num);
        delay_us(10); // 等待write regs完成
        // sync
        do_sync();
        delay_us(10);

        // read regs
        ADS1255_ReadRegs(REG_STATUS, g_adc_rx_buf, reg_num);
        uint8_t is_equal = 1;
        for (uint8_t i = 0; i < reg_num; i++) {
            if ((g_adc_rx_buf[0] & 0x0e) == wreg_vals[0])
                continue;
            if (g_adc_rx_buf[i] != wreg_vals[i]) {
                is_equal = 0;
                break;
            }
        }
        if (is_equal)
            break;
    } while (1);
    delay_us(10);
    SetADS1255_CS;

    delay_us(10);
    SPI1_WriteReadByte(ADS1255_CMD_RDATAC); // read continuously
    delay_us(10);
    SetADS1255_CS;
}

void ADS1255_Reset(void) {
    SPI1_WriteReadByte(ADS1255_CMD_RESET);
}

void ADS1255_WriteRegs(uint8_t reg_addr, const uint8_t *wreg_vals, uint8_t reg_num) {
    // 向reg_addr写reg_num个字节
    SPI1_WriteReadByte(ADS1255_CMD_WREG | reg_addr);
    SPI1_WriteReadByte(reg_num - 1);

    for (uint8_t i = 0; i < reg_num; i++) {
        SPI1_WriteReadByte(wreg_vals[i]);
    }
}

void ADS1255_ReadRegs(uint8_t reg_addr, uint8_t *rreg_vals, uint8_t reg_num) {
    // 停止连续读
    SPI1_WriteReadByte(ADS1255_CMD_SDATAC);

    // 读寄存器
    SPI1_WriteReadByte(ADS1255_CMD_RREG | reg_addr);
    SPI1_WriteReadByte(reg_num - 1);

    delay_ms(10);
    for (uint8_t i = 0; i < reg_num; i++) {
        rreg_vals[reg_addr + i] = SPI1_WriteReadByte(0x00);
    }
}
