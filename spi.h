#ifndef SPI_H
# define  SPI_H

#include "types.h"
static __attribute__((unused)) volatile uint32_t dmastatus;

#define SCREEN_NSS GPIO_PC6
#define UP_NSS set_reg_bits(GPIOC_ODR,1<<6);//{volatile int i; for(i=0;i<10000;i++);}
#define DOWN_NSS clear_reg_bits(GPIOC_ODR,1<<6);//{volatile int i; for(i=0;i<10000;i++);}
#define GPIO_AF_SPI1 5
void spi1_master_send_bytes_async(uint8_t *data, uint32_t length);
void spi1_enable();
void spi1_disable();
void spi1_master(uint8_t baudrate, uint8_t cpol, uint8_t cpha, uint8_t frame,
        uint8_t lsb, uint8_t TI );

void spi1_pin_init();

uint8_t spi1_master_send_byte_sync(uint8_t data);


void spi1_master_init(uint8_t baudrate, uint8_t cpol, uint8_t cpha, uint8_t frame,
        uint8_t lsb, uint8_t TI );

int spi1_is_busy();

#endif
