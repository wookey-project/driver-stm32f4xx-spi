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


#ifdef CONFIG_SPI1
#define SPI_NUMBER 1
#include "generic_spi_header.h"
#line 1 "generic_libspi.h.h"
#include "generic_libspi.h"
#include "generic_spi_footer.h"
#undef SPI_NUMBER
#endif 

#ifdef CONFIG_SPI2
#define SPI_NUMBER 2
#include "generic_spi_header.h"
#line 1 "generic_libspi.h"
#include "generic_libspi.h"
#include "generic_spi_footer.h"
#undef SPI_NUMBER
#endif 

#ifdef CONFIG_SPI3
#include "generic_spi_header.h"
#define SPI_NUMBER 3
#line 1 "generic_libspi.h"
#include "generic_libspi.h"
#include "generic_spi_footer.h"
#undef SPI_NUMBER
#endif 

#ifdef CONFIG_SPI4
#include "generic_spi_header.h"
#define SPI_NUMBER 4
#line 1 "generic_libspi.h"
#include "generic_libspi.h"
#include "generic_spi_footer.h"
#undef SPI_NUMBER
#endif 


#if 0
#define spi_glue(a,b) a ## b
#define spi_master_init(n,baudrate,cpol,cpha,frame,lsb,TI) spi ## n ## _master_init(baudrate,cpol,cpha,frame,lsb,TI)
#define spi_master(n,baudrate,cpol,cpha,frame,lsb,TI) spi ## n ## _master(baudrate,cpol,cpha,frame,lsb,TI)
#define spi_pin_init(n) spi ## n ## _pin_init()
#define spi_master_send_byte_sync(n,data) spi ## n ##_master_send_byte_sync(data)
#define spi_master_send_bytes_async_circular(n,data) spi ## n ##_master_send_byte_sync(data)
#define spi_enable(n) spi ## n ## _enable()
#define spi_disable(n) spi ## n ## _disable()
#endif
#endif
