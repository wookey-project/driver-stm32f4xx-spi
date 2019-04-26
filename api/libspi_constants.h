#ifndef LIBSPI_CONSTANTS_H_
#define LIBSPI_CONSTANTS_H_

/*
 * CAUTION: defines value are set according to the SPI CR1 BR[] value
 * do *not* change each define value
 */
#define    SPI_BAUDRATE_375KHZ  7
#define    SPI_BAUDRATE_750KHZ  6
#define    SPI_BAUDRATE_1500KHZ 5
#define    SPI_BAUDRATE_3MHZ    4
#define    SPI_BAUDRATE_6MHZ    3
#define    SPI_BAUDRATE_12MHZ   2
#define    SPI_BAUDRATE_24MHZ   1
#define    SPI_BAUDRATE_48MHZ   0 /* not for all SPI devices */

#endif
