#ifndef SPI_REGS_H
#define  SPI_REGS_H

#include "api/regutils.h"

#define SPI1BASE			0x40013000

#define r_CORTEX_M_SPI1_CR1     REG_ADDR(SPI1BASE+ 0X0)
#define r_CORTEX_M_SPI1_CR2     REG_ADDR(SPI1BASE+ 0X4)
#define r_CORTEX_M_SPI1_SR      REG_ADDR(SPI1BASE+ 0X8)
#define r_CORTEX_M_SPI1_DR      REG_ADDR(SPI1BASE+ 0XC)
#define r_CORTEX_M_SPI1_CRCPR   REG_ADDR(SPI1BASE+0X10)
#define r_CORTEX_M_SPI1_RXCRCR  REG_ADDR(SPI1BASE+0X14)
#define r_CORTEX_M_SPI1_TXCRCR  REG_ADDR(SPI1BASE+0X18)

#define SPI_MSTR_BIT            0x4 /*(1<<2) */
#define SPI_SPE_BIT             0x40    /*(1<<6) */
#define SPI_BSY_BIT             0x8 /*(1<<7) */
#define SPI_TXE_BIT             1   /*(1<<1) */
#define SPI_TXDMAEN_BIT         1   /*(1<<1) */

#endif                          /* !SPI_REGS_H */
