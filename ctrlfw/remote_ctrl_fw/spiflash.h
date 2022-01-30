#include <stdint.h>

void flash_init();
void flash_deinit();

int flash_read(uint32_t addr, uint8_t *buf, size_t len);
int flash_write_sector(uint32_t addr, uint8_t *buf);
int flash_verify_sector(uint32_t addr, uint8_t *buf);
