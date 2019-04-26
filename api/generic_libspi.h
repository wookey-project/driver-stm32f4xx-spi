void spi_master_send_bytes_async(uint8_t *data, uint32_t length);
void spi_enable();
void spi_disable();
void spi_master(uint8_t baudrate, uint8_t cpol, uint8_t cpha, uint8_t frame,
        uint8_t lsb, uint8_t TI );

void spi_pin_init();

uint8_t spi_master_send_byte_sync(uint8_t data);
void spi_master_send_bytes_async_circular(uint8_t *data, uint32_t mylengthdata, uint32_t length);


void spi_master_init(uint8_t baudrate, uint8_t cpol, uint8_t cpha, uint8_t frame,
        uint8_t lsb, uint8_t TI );

int spi_is_busy();
uint8_t spi_early_init();
uint8_t spi_init(uint8_t baudrate);

mbed_error_t spi_set_baudrate(uint8_t baudrate);
uint8_t      spi_get_baudrate(void);
