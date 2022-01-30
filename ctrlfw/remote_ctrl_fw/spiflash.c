#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/spi.h"

#include "spiflash.h"
#include "config.h"

// https://github.com/raspberrypi/pico-examples/blob/master/spi/spi_flash/spi_flash.c

#define FLASH_PAGE_SIZE        256
#define FLASH_SECTOR_SIZE      4096

#define FLASH_CMD_PAGE_PROGRAM 0x02
#define FLASH_CMD_READ         0x03
#define FLASH_CMD_STATUS       0x05
#define FLASH_CMD_WRITE_EN     0x06
#define FLASH_CMD_SECTOR_ERASE 0x20

#define FLASH_STATUS_BUSY_MASK 0x01

static inline void _cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(FLASH_CS_PIN, 0);
    asm volatile("nop \n nop \n nop");
}

static inline void _cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(FLASH_CS_PIN, 1);
    asm volatile("nop \n nop \n nop");
}

void _flash_write_enable() {
    _cs_select();
    uint8_t cmd = FLASH_CMD_WRITE_EN;
    spi_write_blocking(FLASH_SPI, &cmd, 1);
    _cs_deselect();
}

void _flash_wait_done() {
    uint8_t status;
    do {
        _cs_select();
        uint8_t buf[2] = {FLASH_CMD_STATUS, 0};
        spi_write_read_blocking(FLASH_SPI, buf, buf, 2);
        _cs_deselect();
        status = buf[1];
    } while (status & FLASH_STATUS_BUSY_MASK);
}

void _flash_sector_erase(uint32_t addr) {
    uint8_t cmdbuf[4] = {
            FLASH_CMD_SECTOR_ERASE,
            addr >> 16,
            addr >> 8,
            addr
    };
    _flash_write_enable();
    _cs_select();
    spi_write_blocking(FLASH_SPI, cmdbuf, 4);
    _cs_deselect();
    _flash_wait_done();
}

void _flash_page_program(uint32_t addr, uint8_t data[]) {
    uint8_t cmdbuf[4] = {
            FLASH_CMD_PAGE_PROGRAM,
            addr >> 16,
            addr >> 8,
            addr
    };
    _flash_write_enable();
    _cs_select();
    spi_write_blocking(FLASH_SPI, cmdbuf, 4);
    spi_write_blocking(FLASH_SPI, data, FLASH_PAGE_SIZE);
    _cs_deselect();
    _flash_wait_done();
}

void flash_init() {
    spi_init(FLASH_SPI, 1000 * 1000);
    gpio_set_function(FLASH_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(FLASH_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(FLASH_MISO_PIN, GPIO_FUNC_SPI);

    gpio_init(FLASH_CS_PIN);
    gpio_put(FLASH_CS_PIN, 1);
    gpio_set_dir(FLASH_CS_PIN, true); // output
}

void flash_deinit() {
    // Set all the I/Os to be inputs so they are Hi-Z

    gpio_set_function(FLASH_SCK_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(FLASH_SCK_PIN, false);

    gpio_set_function(FLASH_MOSI_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(FLASH_MOSI_PIN, false);

    gpio_set_function(FLASH_MISO_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(FLASH_MISO_PIN, false);

    gpio_set_function(FLASH_CS_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(FLASH_CS_PIN, false);
}

int flash_read(uint32_t addr, uint8_t *buf, size_t len) {
    _cs_select();
    uint8_t cmdbuf[4] = {
            FLASH_CMD_READ,
            addr >> 16,
            addr >> 8,
            addr
    };
    spi_write_blocking(FLASH_SPI, cmdbuf, 4);
    spi_read_blocking(FLASH_SPI, 0, buf, len);
    _cs_deselect();

    return 0;
}

// Write a 4KByte sector
int flash_write_sector(uint32_t addr, uint8_t *buf) {
    if ((addr & (FLASH_SECTOR_SIZE - 1)) != 0) return -1; // Unaligned address

    _flash_sector_erase(addr);
    for (uint32_t offset = 0; offset < FLASH_SECTOR_SIZE; offset += FLASH_PAGE_SIZE) {
        _flash_page_program(addr + offset, buf + offset);
    }

    return 0;
}

// Verify contents of 4KByte sector
int flash_verify_sector(uint32_t addr, uint8_t *buf) {
    if ((addr & (FLASH_SECTOR_SIZE - 1)) != 0) return -1; // Unaligned address

    uint8_t tmp[FLASH_SECTOR_SIZE];
    flash_read(addr, tmp, FLASH_SECTOR_SIZE);

    if (memcmp(tmp, buf, FLASH_SECTOR_SIZE) != 0) return 1;
    return 0;
}

