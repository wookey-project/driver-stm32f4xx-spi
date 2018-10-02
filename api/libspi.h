#ifndef SPI_H
# define  SPI_H

#include "spi_regs.h"
#include "api/types.h"
//static __attribute__((unused)) volatile uint32_t dmastatus;

#define SCREEN_NSS GPIO_PC6
#define UP_NSS sys_cfg(CFG_GPIO_SET,(uint8_t)((('C' - 'A') << 4) + 6),1);
 //set_reg_bits(GPIOC_ODR,1<<6);//{volatile int i; for(i=0;i<10000;i++);}
#define DOWN_NSS sys_cfg(CFG_GPIO_SET,(uint8_t)((('C' - 'A') << 4) + 6),0);
 //clear_reg_bits(GPIOC_ODR,1<<6);//{volatile int i; for(i=0;i<10000;i++);}
//#define GPIO_AF_SPI1 5
void spi1_master_send_bytes_async(uint8_t *data, uint32_t length);
void spi1_enable();
void spi1_disable();
void spi1_master(uint8_t baudrate, uint8_t cpol, uint8_t cpha, uint8_t frame,
        uint8_t lsb, uint8_t TI );

void spi1_pin_init();

uint8_t spi1_master_send_byte_sync(uint8_t data);
void spi1_master_send_bytes_async_circular(uint8_t *data, uint32_t mylengthdata, uint32_t length);


void spi1_master_init(uint8_t baudrate, uint8_t cpol, uint8_t cpha, uint8_t frame,
        uint8_t lsb, uint8_t TI );

int spi1_is_busy();
uint8_t spi1_early_init();
uint8_t spi1_init();

#define spi_glue(a,b) a ## b
#define spi_master_init(n,baudrate,cpol,cpha,frame,lsb,TI) spi ## n ## _master_init(baudrate,cpol,cpha,frame,lsb,TI)
#define spi_master(n,baudrate,cpol,cpha,frame,lsb,TI) spi ## n ## _master(baudrate,cpol,cpha,frame,lsb,TI)
#define spi_pin_init(n) spi ## n ## _pin_init()
#define spi_master_send_byte_sync(n,data) spi ## n ##_master_send_byte_sync(data)
#define spi_master_send_bytes_async_circular(n,data) spi ## n ##_master_send_byte_sync(data)
#define spi_enable(n) spi ## n ## _enable()
#define spi_disable(n) spi ## n ## _disable()


#endif
