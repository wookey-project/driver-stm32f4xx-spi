
static __attribute__((unused)) volatile uint32_t spi_dmastatus;


/*
 * This lib requires:
 * BUS permission
 * DMA permission
 */
static const char spiname[5] = strnumber;



/***********************************************
 * spi block startup function
 ***********************************************/

static void spi_power_up(void)
{
}

static void spi_enabling_irqs(void)
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
void SPI_IRQHandler(uint8_t irq __UNUSED, // IRQ number
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

void spi_complete_callback_circular(uint8_t irq, uint32_t status);

static const struct {int port; int num;} spi_configuration[]={ /*NSS*/ {GPIO_PC,6},\
						        /*CLK*/ {GPIO_PB,3},\
						        /*MISO*/{GPIO_PA,6},\
						        /*MOSI*/{GPIO_PA,7}};
static const uint32_t spi_role=SPI_MSTR_BIT;
static const uint32_t spi_baudrate=2;
static const uint32_t spi_cpol=0;
static const uint32_t spi_cpha=0;
static const uint32_t spi_frame=0;
static const uint32_t spi_lsb=0;//LSB MODE
static const uint32_t spi_TI_mode=0;//Motorola not TI frame
static dma_t spi_dma={0};
static int spi_dmadesc;

uint8_t spi_early_init()
{
    uint8_t ret = 0;

    /*******************************
     * first, spi device declaration
     *******************************/
    device_t dev;
    memset(&dev, 0, sizeof(device_t));
    int devdesc;

    /*
     * declare the spi device, during initialization phase
     * This function create a device_t, fullfill it, and execute a
     * sys_init(INIT_DEVACCESS) syscall.
     */
    memset(&dev,sizeof(device_t),0);
    memcpy(dev.name, spiname, strlen(spiname));
    dev.address = SPIBASE;
    dev.size = 0x400; /*FIXME: 0x400????? just need 0x20 */
    dev.irq_num = 1;

    /* IRQ configuration */
    dev.irqs[0].handler = SPI_IRQHandler;
    dev.irqs[0].irq =SPI_IRQ; //59; /* starting with STACK 43 + 16*/
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
    dev.gpios[0].mode       = (spi_role==SPI_MSTR_BIT?GPIO_PIN_OUTPUT_MODE:GPIO_PIN_INPUT_MODE);
    dev.gpios[0].pupd       = GPIO_PULLUP;
    dev.gpios[0].type       = GPIO_PIN_OTYPER_PP;
    dev.gpios[0].speed      = GPIO_PIN_VERY_HIGH_SPEED;

    /* SPI_CK is on PB3 */
    dev.gpios[1].mask         = GPIO_MASK_SET_MODE | GPIO_MASK_SET_PUPD | GPIO_MASK_SET_TYPE | GPIO_MASK_SET_SPEED | GPIO_MASK_SET_AFR;
    dev.gpios[1].kref.port    = spi_configuration[1].port;
    dev.gpios[1].kref.pin     = spi_configuration[1].num;
    dev.gpios[1].mode         = GPIO_PIN_ALTERNATE_MODE;
    dev.gpios[1].pupd         = GPIO_PULLUP;
    dev.gpios[1].speed        = GPIO_PIN_VERY_HIGH_SPEED;
    dev.gpios[1].afr          = GPIO_AF_SPI;

    /* SPI_MISO is on PA6 */
    dev.gpios[2].mask         = GPIO_MASK_SET_MODE | GPIO_MASK_SET_PUPD | GPIO_MASK_SET_TYPE | GPIO_MASK_SET_SPEED | GPIO_MASK_SET_AFR;
    dev.gpios[2].kref.port    = spi_configuration[2].port;
    dev.gpios[2].kref.pin     = spi_configuration[2].num;
    dev.gpios[2].mode         = GPIO_PIN_ALTERNATE_MODE;
    dev.gpios[2].pupd         = GPIO_NOPULL;
    dev.gpios[2].speed        = GPIO_PIN_VERY_HIGH_SPEED;
    dev.gpios[2].afr          = GPIO_AF_SPI;

    /* SPI_MOSI is on PA7 */
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

    spi_dma.channel = DMA2_CHANNEL_SPI;
    spi_dma.dir = MEMORY_TO_PERIPHERAL; /* write by default */
    spi_dma.in_addr = (physaddr_t) 0; /* to set later via DMA_RECONF */
    spi_dma.out_addr = (volatile physaddr_t)r_CORTEX_M_SPI_DR;
    spi_dma.in_prio = DMA_PRI_HIGH;
    spi_dma.dma = DMA2;
    spi_dma.size = 0; /* to set later via DMA_RECONF */

    spi_dma.stream = 3;

    spi_dma.mode = DMA_CIRCULAR_MODE;
    spi_dma.mem_inc = 1;
    spi_dma.dev_inc = 0;
    spi_dma.datasize = DMA_DS_BYTE;
    spi_dma.mem_burst = DMA_BURST_SINGLE;//DMA_BURST_INC1;
    spi_dma.dev_burst = DMA_BURST_SINGLE;//DMA_BURST_INC1;
    spi_dma.flow_control = DMA_FLOWCTRL_DMA;
    spi_dma.in_handler = (user_dma_handler_t) spi_complete_callback_circular;
    spi_dma.out_handler = (user_dma_handler_t) spi_complete_callback_circular;
#if 0
    ret |= sys_init(INIT_DMA, &dma, &dmadesc);
    if(ret)
      printf("%s:%d %d\n",__FILE__,__LINE__,ret);
#endif

    return ret;
}

uint8_t spi_init()
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

  write_reg_value(r_CORTEX_M_SPI_CR1, ((spi_frame&1)<<11)|(1<<14)|((spi_lsb&1)<<7)|
      ((spi_baudrate&7)<<3)|((spi_cpol&1)<<1)|(spi_cpha&1));

  /* SSM enabled (bit 9) = 1
     Bit 8 = SSI its value is used instead of NSS to check possibility to send data
  */
  set_reg_bits(r_CORTEX_M_SPI_CR1,(1<<9)|(1<<8));

  /*Set the FRF bit in SPI_CR2 to select TI or motorola mode*/
  if(spi_TI_mode)
    set_reg_bits(r_CORTEX_M_SPI_CR2, ((spi_TI_mode&1)<<4));
  else
    clear_reg_bits(r_CORTEX_M_SPI_CR2, ((spi_TI_mode&1)<<4));

  /*Set MSTR and SPE in SPI_CR1 */
  set_reg_bits(r_CORTEX_M_SPI_CR1, spi_role);
  set_reg_bits(r_CORTEX_M_SPI_CR1, SPI_SPE_BIT);/* SPI Enable */

    return 0;
}

/* other functions abstracting spi interaction (low level commands execution) */


int spi_is_busy()
{
   return //(!(read_reg_value(r_CORTEX_M_SPI_SR)&SPI_TXE_BIT))
            //    && !
		(read_reg_value(r_CORTEX_M_SPI_SR) & SPI_BSY_BIT);
}

/* WARNING This function assumes taht NSS is already low !!!*/

uint8_t spi_master_send_byte_sync(uint8_t data)
{
       uint8_t retval=0;
        /* Wait for SPI to be ready */
        while(read_reg_value(r_CORTEX_M_SPI_SR) & SPI_BSY_BIT);
        /* Write transmit buffer */
        write_reg_value(r_CORTEX_M_SPI_DR,data);
        /* Wait for completion */
        while(!(read_reg_value(r_CORTEX_M_SPI_SR) & SPI_TXE_BIT));
        while(read_reg_value(r_CORTEX_M_SPI_SR) & SPI_BSY_BIT);
        /* Read SPI_DR */
        retval=(uint8_t)read_reg_value(r_CORTEX_M_SPI_DR);
        return retval;
}


static uint32_t localcpt;
static uint32_t totallength,lengthdata;
void spi_master_send_bytes_async_circular(uint8_t *data, uint32_t mydatalength, uint32_t length)
{
        spi_dma.in_addr=(uint32_t)data;
	spi_dma.size=mydatalength;
        /*Wait for any ongoing tranfert to complete */
        while(spi_dmastatus==PHTBUSY);

        set_reg_bits(r_CORTEX_M_SPI_CR2,SPI_TXDMAEN_BIT);
	localcpt=0;
	totallength=length;
	lengthdata=mydatalength;

	spi_dmastatus=PHTBUSY;
	sys_cfg(CFG_DMA_RECONF, &spi_dma, DMA_RECONF_BUFIN | DMA_RECONF_BUFSIZE
                | DMA_RECONF_MODE ,spi_dmadesc);

        set_reg_bits(r_CORTEX_M_SPI_CR2,(1<<1));
        while(spi_dmastatus!=PHTIDLE);
//	  sys_yield();
}
void spi_complete_callback_circular(uint8_t __attribute__((unused)) irq, uint32_t status)
{
//KERNEL C = must shift status value
//KERNEL ADA = value already shifted
  status >>= 22;
  if((status&(0x20)))
  {
    localcpt+=lengthdata;
    if(localcpt>=totallength)
    {

      sys_cfg(CFG_DMA_DISABLE,spi_dmadesc);
      while(!(read_reg_value(r_CORTEX_M_SPI_SR)&SPI_TXE_BIT));
      while(read_reg_value(r_CORTEX_M_SPI_SR)&SPI_BSY_BIT);
      clear_reg_bits(r_CORTEX_M_SPI_CR2,SPI_TXDMAEN_BIT);
      spi_dmastatus=PHTIDLE;
    }
  } /* Wait for everything to be fully completed */
  /* See P898  Note*/
}

void spi_reset_dma()
{
  spi_dmastatus=PHTIDLE;
}

void spi_enable()
{
        /* Enable SPI byte setting SPE */
	set_reg_bits(r_CORTEX_M_SPI_CR1,SPI_SPE_BIT);
}
void spi_disable()
{
        /* Disable SPI byte clearing SPE */
	clear_reg_bits(r_CORTEX_M_SPI_CR1,SPI_SPE_BIT);
}

