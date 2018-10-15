#include "api/types.h"
#include "kernel/exported/devices.h"
#include "kernel/exported/dmas.h"
#include "api/libspi.h"
#include "api/print.h"
#include "api/regutils.h"
#include "spi.h"
#include "api/spi_regs.h"
#include "api/syscall.h"

#define GPIO_AF_SPI 5
#define DMA2_CHANNEL_SPI 3
#define DMA2 2
#define PHTIDLE 0
#define PHTBUSY 1

static __attribute__((unused)) volatile uint32_t spi_dmastatus;


/*
 * This lib requires:
 * BUS permission
 * DMA permission
 */
static const char spiname[4] = "spi1";



/***********************************************
 * spi block startup function
 ***********************************************/

static void power_up(void)
{
}

static void enabling_irqs(void)
{
}


/*****************************************
 * IRQ Handlers
 *
 * prototype for irq handlers is:
 * static void my_irq_handler(uint8_t irq, // IRQ number
 *                           uint32_t sr   // content of posthook.status,
 *                           uint32_t dr   // content of posthook.data)
 *
 *****************************************/
void SPI1_IRQHandler(uint8_t irq __UNUSED, // IRQ number
                     uint32_t sr __UNUSED,  // content of posthook.status,
                     uint32_t dr)  // content of posthook.data)
{
	uint32_t __attribute__((unused)) mask = dr;

}

/********************************************
 * Various utility functions
 ********************************************/



/*****************************************
 * Initialization functions
 *****************************************/

void spi1_complete_callback_circular(uint8_t irq, uint32_t status);

static const struct {int port; int num;} spi_configuration[]={ /*NSS*/ {GPIO_PC,6},\
						        /*CLK*/ {GPIO_PB,3},\
						        /*MISO*/{GPIO_PA,6},\
						        /*MOSI*/{GPIO_PA,7}};
static const uint32_t role=SPI_MSTR_BIT;
static const uint32_t baudrate=2;
static const uint32_t cpol=0;
static const uint32_t cpha=0;
static const uint32_t frame=0;
static const uint32_t lsb=0;//LSB MODE
static const uint32_t TI_mode=0;//Motorola not TI frame
static dma_t dma={0};
static int dmadesc;
uint8_t spi1_early_init()
{
    uint8_t ret = 0;

    /*******************************
     * first, spi device declaration
     *******************************/
    device_t dev = { 0 };
    int devdesc;

    /*
     * declare the spi device, during initialization phase
     * This function create a device_t, fullfill it, and execute a
     * sys_init(INIT_DEVACCESS) syscall.
     */
    memcpy(dev.name, spiname, strlen(spiname));
    dev.address = SPI1BASE;
    dev.size = 0x400; /*FIXME: 0x400????? just need 0x20 */
    dev.irq_num = 1;

    /* IRQ configuration */
    dev.irqs[0].handler = SPI1_IRQHandler;
    dev.irqs[0].irq =SPI1_IRQ; //59; /* starting with STACK 43 + 16*/
    dev.irqs[0].mode = IRQ_ISR_STANDARD; /* if ISR force MT immediat execution, use FORCE_MAINTHREAD instead of STANDARD, and activate FISR permission */
    
    dev.irqs[0].posthook.status = 0x0008; /* SR is first read */
    dev.irqs[0].posthook.data = 0x000c; /* Data reg  is 2nd read */


    /*
     * IRQ posthook configuration
     * The posthook is executed at the end of the IRQ handler mode, *before* the ISR.
     * It permit to clean potential status registers (or others) that may generate IRQ loops
     * while the ISR has not been executed.
     * register read can be saved into 'status' and 'data' and given to the ISR in 'sr' and 'dr' argument
     */
    dev.irqs[0].posthook.action[0].instr = IRQ_PH_READ;
    dev.irqs[0].posthook.action[0].read.offset = 0x08/* SPI_SR register offset */;

    /* Now let's configure the GPIOs */
    dev.gpio_num = 4;

    /* Configure NSS to be on GPIO_PC6 */
    dev.gpios[0].mask       = GPIO_MASK_SET_MODE | GPIO_MASK_SET_PUPD \
				| GPIO_MASK_SET_TYPE | GPIO_MASK_SET_SPEED \
				| GPIO_MASK_SET_AFR;
    dev.gpios[0].kref.port  = spi_configuration[0].port;
    dev.gpios[0].kref.pin   = spi_configuration[0].num;
    dev.gpios[0].mode       = (role==SPI_MSTR_BIT?GPIO_PIN_OUTPUT_MODE:GPIO_PIN_INPUT_MODE);
    dev.gpios[0].pupd       = GPIO_PULLUP;
    dev.gpios[0].type       = GPIO_PIN_OTYPER_PP;
    dev.gpios[0].speed      = GPIO_PIN_VERY_HIGH_SPEED;

    /* SPI1_CK is on PB3 */ 
    dev.gpios[1].mask         = GPIO_MASK_SET_MODE | GPIO_MASK_SET_PUPD | GPIO_MASK_SET_TYPE | GPIO_MASK_SET_SPEED | GPIO_MASK_SET_AFR;
    dev.gpios[1].kref.port    = spi_configuration[1].port;
    dev.gpios[1].kref.pin     = spi_configuration[1].num;
    dev.gpios[1].mode         = GPIO_PIN_ALTERNATE_MODE;
    dev.gpios[1].pupd         = GPIO_PULLUP;
    dev.gpios[1].speed        = GPIO_PIN_VERY_HIGH_SPEED;
    dev.gpios[1].afr          = GPIO_AF_SPI;

    /* SPI1_MISO is on PA6 */ 
    dev.gpios[2].mask         = GPIO_MASK_SET_MODE | GPIO_MASK_SET_PUPD | GPIO_MASK_SET_TYPE | GPIO_MASK_SET_SPEED | GPIO_MASK_SET_AFR;
    dev.gpios[2].kref.port    = spi_configuration[2].port;
    dev.gpios[2].kref.pin     = spi_configuration[2].num;
    dev.gpios[2].mode         = GPIO_PIN_ALTERNATE_MODE;
    dev.gpios[2].pupd         = GPIO_NOPULL;
    dev.gpios[2].speed        = GPIO_PIN_VERY_HIGH_SPEED;
    dev.gpios[2].afr          = GPIO_AF_SPI;
	
    /* SPI1_MOSI is on PA7 */ 
    dev.gpios[3].mask         = GPIO_MASK_SET_MODE | GPIO_MASK_SET_PUPD | GPIO_MASK_SET_TYPE | GPIO_MASK_SET_SPEED | GPIO_MASK_SET_AFR;
    dev.gpios[3].kref.port    = spi_configuration[3].port;
    dev.gpios[3].kref.pin     = spi_configuration[3].num;
    dev.gpios[3].mode         = GPIO_PIN_ALTERNATE_MODE;
    dev.gpios[3].pupd         = GPIO_NOPULL;
    dev.gpios[3].speed        = GPIO_PIN_VERY_HIGH_SPEED;
    dev.gpios[3].afr          = GPIO_AF_SPI;

    
    ret = sys_init(INIT_DEVACCESS, &dev, &devdesc);
    if(ret)
      printf("%s:%d %d\n",__FILE__,__LINE__,ret);
   
    dma.channel = DMA2_CHANNEL_SPI;
    dma.dir = MEMORY_TO_PERIPHERAL; /* write by default */
    dma.in_addr = (physaddr_t) 0; /* to set later via DMA_RECONF */
    dma.out_addr = (volatile physaddr_t)r_CORTEX_M_SPI1_DR;
    dma.in_prio = DMA_PRI_HIGH;
    dma.dma = DMA2;
    dma.size = 0; /* to set later via DMA_RECONF */

    dma.stream = 3;

    dma.mode = DMA_CIRCULAR_MODE;
    dma.mem_inc = 1;
    dma.dev_inc = 0;
    dma.datasize = DMA_DS_BYTE;
    dma.mem_burst = DMA_BURST_SINGLE;//DMA_BURST_INC1;
    dma.dev_burst = DMA_BURST_SINGLE;//DMA_BURST_INC1;
    dma.flow_control = DMA_FLOWCTRL_DMA;
    dma.in_handler = (user_dma_handler_t) spi1_complete_callback_circular;
    dma.out_handler = (user_dma_handler_t) spi1_complete_callback_circular;
#if 0
    ret |= sys_init(INIT_DMA, &dma, &dmadesc);
    if(ret)
      printf("%s:%d %d\n",__FILE__,__LINE__,ret);
#endif

    return ret;
}

uint8_t spi1_init()
{
  /*
   * configure the spi device, once it is mapped in task memory
   * This function must be executed *after* sys_init(INIT_DONE).
   */



  /* CF P885 RM0090 
     BR[2:0] = CPOL & CPHA
     defaulting to LSB First
     Defaulting to BIDIMODE=0 
     FIXME Handle TI mode also
     bit 14 = BIIOE ,enable output*/

  write_reg_value(r_CORTEX_M_SPI1_CR1, ((frame&1)<<11)|(1<<14)|((lsb&1)<<7)|
      ((baudrate&7)<<3)|((cpol&1)<<1)|(cpha&1));

  /* SSM enabled (bit 9) = 1
     Bit 8 = SSI its value is used instead of NSS to check possibility to send data 
  */
  set_reg_bits(r_CORTEX_M_SPI1_CR1,(1<<9)|(1<<8));

  /*Set the FRF bit in SPI1_CR2 to select TI or motorola mode*/
  if(TI_mode)
    set_reg_bits(r_CORTEX_M_SPI1_CR2, ((TI_mode&1)<<4));
  else
    clear_reg_bits(r_CORTEX_M_SPI1_CR2, ((TI_mode&1)<<4));
  
  /*Set MSTR and SPE in SPI1_CR1 */
  set_reg_bits(r_CORTEX_M_SPI1_CR1, role);
  set_reg_bits(r_CORTEX_M_SPI1_CR1, SPI_SPE_BIT);/* SPI Enable */

    return 0;
}

/* other functions abstracting spi interaction (low level commands execution) */


int spi1_is_busy()
{
   return //(!(read_reg_value(r_CORTEX_M_SPI1_SR)&SPI_TXE_BIT))
            //    && !
		(read_reg_value(r_CORTEX_M_SPI1_SR) & SPI_BSY_BIT);
}

/* WARNING This function assumes taht NSS is already low !!!*/

uint8_t spi1_master_send_byte_sync(uint8_t data)
{
       uint8_t retval=0;
        /* Wait for SPI to be ready */
        while(read_reg_value(r_CORTEX_M_SPI1_SR) & SPI_BSY_BIT);
        /* Write transmit buffer */
        write_reg_value(r_CORTEX_M_SPI1_DR,data);
        /* Wait for completion */
        while(!(read_reg_value(r_CORTEX_M_SPI1_SR) & SPI_TXE_BIT));
        while(read_reg_value(r_CORTEX_M_SPI1_SR) & SPI_BSY_BIT);
        /* Read SPI_DR */
        retval=(uint8_t)read_reg_value(r_CORTEX_M_SPI1_DR);
        return retval;
}


static uint32_t localcpt;
static uint32_t totallength,lengthdata;
void spi1_master_send_bytes_async_circular(uint8_t *data, uint32_t mydatalength, uint32_t length)
{
        dma.in_addr=(uint32_t)data;
	dma.size=mydatalength;
        /*Wait for any ongoing tranfert to complete */
        while(spi_dmastatus==PHTBUSY);

        set_reg_bits(r_CORTEX_M_SPI1_CR2,SPI_TXDMAEN_BIT);
	localcpt=0;
	totallength=length;
	lengthdata=mydatalength;

	spi_dmastatus=PHTBUSY;
	sys_cfg(CFG_DMA_RECONF, &dma, DMA_RECONF_BUFIN | DMA_RECONF_BUFSIZE | DMA_RECONF_MODE ,dmadesc);

        set_reg_bits(r_CORTEX_M_SPI1_CR2,(1<<1));
        while(spi_dmastatus!=PHTIDLE);
//	  sys_yield();
}
void spi1_complete_callback_circular(uint8_t __attribute__((unused)) irq, uint32_t status)
{
//KERNEL C = must shift status value
//KERNEL ADA = value already shifted
  status >>= 22; 
  if((status&(0x20)))
  {
    localcpt+=lengthdata;
    if(localcpt>=totallength)
    {

      sys_cfg(CFG_DMA_DISABLE,dmadesc);
      while(!(read_reg_value(r_CORTEX_M_SPI1_SR)&SPI_TXE_BIT));
      while(read_reg_value(r_CORTEX_M_SPI1_SR)&SPI_BSY_BIT);
      clear_reg_bits(r_CORTEX_M_SPI1_CR2,SPI_TXDMAEN_BIT);
      spi_dmastatus=PHTIDLE;
    }
  } /* Wait for everything to be fully completed */
  /* See P898  Note*/
}

void spi1_reset_dma()
{
  spi_dmastatus=PHTIDLE;
}

void spi1_enable()
{
        /* Enable SPI byte setting SPE */
	set_reg_bits(r_CORTEX_M_SPI1_CR1,SPI_SPE_BIT);
}
void spi1_disable()
{
        /* Disable SPI byte clearing SPE */
	clear_reg_bits(r_CORTEX_M_SPI1_CR1,SPI_SPE_BIT);
}

