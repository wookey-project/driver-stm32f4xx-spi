void spi_master_send_bytes_async(uint8_t *data, uint32_t length);

uint8_t spi_master_recv_byte_sync(void);

uint8_t spi_master_send_byte_sync(uint8_t data);

void spi_master_send_bytes_async_circular(uint8_t *data, uint32_t mylengthdata, uint32_t length);

void spi_enable();

void spi_disable();

int spi_is_busy();

mbed_error_t spi_set_baudrate(uint8_t baudrate);

uint8_t      spi_get_baudrate(void);

uint8_t spi_early_init();
uint8_t spi_init(uint8_t baudrate);
