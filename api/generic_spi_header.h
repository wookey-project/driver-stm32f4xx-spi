/*
*
*   Tools
*
*/
#define spi_glue(a,b) a ## b
#define spi_xglue(a,b) spi_glue(a, b)
#define spi_triglue(a,b,c) a ## b ## c
#define spi_xtriglue(a,b,c) spi_triglue(a,b,c)

#define str(a) #a
#define xstr(a) str(a)
#define spi_xgluestr(a,b) xstr(spi_glue(a,b))
/*
*
*
*  Function renaming : if SPI_NUMBER is 1 the function foo is renamed foo1
*                      and  variables VAR becomes VAR1
*
*
*/
#define spi_master_init spi_xtriglue(spi,SPI_NUMBER,_master_init)
#define spi_master spi_xtriglue(spi,SPI_NUMBER,_master)
#define spi_pin_init spi_xtriglue(spi,SPI_NUMBER,_pin_init)
#define spi_master_send_byte_sync spi_xtriglue(spi,SPI_NUMBER,_master_send_byte_sync)
#define spi_master_send_bytes_async_circular spi_xtriglue(spi,SPI_NUMBER,_master_send_byte_async_circular)
#define spi_enable spi_xtriglue(spi,SPI_NUMBER, _enable)
#define spi_disable spi_xtriglue(spi,SPI_NUMBER,_disable)
#define SPI_IRQHandler spi_xtriglue(SPI ,SPI_NUMBER,_IRQHandler)
#define spi_complete_callback_circular spi_xtriglue(spi,SPI_NUMBER,_complete_callback_circular)
#define spi_early_init spi_xtriglue(spi ,SPI_NUMBER ,_early_init)
#define spi_reset_dma spi_xtriglue(spi,SPI_NUMBER,_reset_dma)
#define spi_is_busy spi_xtriglue(spi ,SPI_NUMBER ,_is_busy)
#define spi_init spi_xtriglue(spi ,SPI_NUMBER , _init)
#define SPIBASE spi_xtriglue(SPI ,SPI_NUMBER ,BASE)
#define SPI_IRQ spi_xtriglue (SPI ,SPI_NUMBER ,_IRQ)
#define spi_power_up spi_xtriglue (spi ,SPI_NUMBER ,_power_up)
#define spi_enabling_irqs spi_xtriglue (spi ,SPI_NUMBER ,_enabling_irqs)
#define spi_configuration spi_xtriglue (spi ,SPI_NUMBER ,_configuration)
#define spi_set_baudrate spi_xtriglue (spi ,SPI_NUMBER ,_set_baudrate)
#define spi_get_baudrate spi_xtriglue (spi ,SPI_NUMBER ,_get_baudrate)
#define spi_set_endianness spi_xtriglue (spi ,SPI_NUMBER ,_set_endianness)
#define spi_set_frame spi_xtriglue (spi ,SPI_NUMBER ,_set_frame)
#define spi_set_cpol_cpha spi_xtriglue (spi ,SPI_NUMBER ,_set_cpol_cpha)

/*
*
*
*  Registers renaming
*
*
*/
#define r_CORTEX_M_SPI_DR spi_xtriglue(r_CORTEX_M_SPI ,SPI_NUMBER ,_DR)
#define r_CORTEX_M_SPI_CR1 spi_xtriglue(r_CORTEX_M_SPI ,SPI_NUMBER ,_CR1)
#define r_CORTEX_M_SPI_CR2 spi_xtriglue(r_CORTEX_M_SPI ,SPI_NUMBER ,_CR2)
#define r_CORTEX_M_SPI_SR spi_xtriglue(r_CORTEX_M_SPI ,SPI_NUMBER ,_SR)

/*
* Global variable renaming
*/
#define spi_dmastatus spi_xtriglue(spi, SPI_NUMBER, _dmastatus)

#define strnumber spi_xgluestr(spi, SPI_NUMBER )
#define spiname spi_xtriglue(spi,SPI_NUMBER,name)
#define spi_role spi_xtriglue(spi,SPI_NUMBER,_role)
#define spi_baudrate spi_xtriglue(spi,SPI_NUMBER,_baudrate)
#define spi_cpol spi_xtriglue(spi,SPI_NUMBER,_cpol)
#define spi_cpha spi_xtriglue(spi,SPI_NUMBER,_cpha)
#define spi_frame spi_xtriglue(spi,SPI_NUMBER,_frame)
#define spi_lsb spi_xtriglue(spi,SPI_NUMBER,_lsb)
#define spi_TI_mode spi_xtriglue(spi,SPI_NUMBER,_TI_mode)
#define spi_dma spi_xtriglue(spi,SPI_NUMBER,_dma)
#define spi_dmadesc spi_xtriglue(spi,SPI_NUMBER,_dmadesc)
#define spi_dev_infos spi_xtriglue(spi,SPI_NUMBER,_dev_infos)

#define CONFIG_SPI_ROLE_MASTER spi_xtriglue(CONFIG_SPI,SPI_NUMBER,_ROLE_MASTER)

