#include "api/types.h"
#include "kernel/exported/devices.h"
#include "kernel/exported/dmas.h"
#include "api/print.h"
#include "api/regutils.h"
#include "spi_regs.h"
#include "api/syscall.h"

#define GPIO_AF_SPI 5
#define DMA2_CHANNEL_SPI 3
#define DMA2 2
#define PHTIDLE 0
#define PHTBUSY 1





#ifdef CONFIG_SPI1
#define SPI_NUMBER 1
#include "api/generic_spi_header.h"
#line 1 "spi_generic.h"
#include "spi_generic.h"
#include "api/generic_spi_footer.h"
#undef SPI_NUMBER
#endif

#ifdef CONFIG_SPI2
#define SPI_NUMBER 2
#include "api/generic_spi_header.h"
#line 1 "spi_generic.h"
#include "spi_generic.h"
#include "api/generic_spi_footer.h"
#undef SPI_NUMBER
#endif

#ifdef CONFIG_SPI3
#include "api/generic_spi_header.h"
#define SPI_NUMBER 3
#line 1 "spi_generic.h"
#include "spi_generic.h"
#include "api/generic_spi_footer.h"
#undef SPI_NUMBER
#endif

#ifdef CONFIG_SPI4
#include "api/generic_spi_header.h"
#define SPI_NUMBER 4
#line 1 "spi_generic.h"
#include "spi_generic.h"
#include "api/generic_spi_footer.h"
#undef SPI_NUMBER
#endif

