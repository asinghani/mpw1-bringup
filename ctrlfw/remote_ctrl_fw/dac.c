#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "config.h"
#include "dac.h"

void init_dac_i2c() {
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(I2C0_SCL, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SCL);
    gpio_pull_up(I2C0_SDA);

    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(I2C1_SCL, GPIO_FUNC_I2C);
    gpio_set_function(I2C1_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C1_SCL);
    gpio_pull_up(I2C1_SDA);
}

int set_voltage_caravel(uint16_t dat) {
    uint8_t wr_buf[3] = {0x10, (dat >> 8) & 0xFF, dat & 0xFF};

    if (i2c_write_timeout_us(
            DAC_CARAVEL_BUS,
            DAC_CARAVEL_ADDR,
            wr_buf, 3, true,
            100000) != 3) return -1;

    return 0;
}

int init_dac_caravel() {
    uint8_t wr_buf[3] = {0x11, 0x20, 0x00};

    if (i2c_write_timeout_us(
            DAC_CARAVEL_BUS,
            DAC_CARAVEL_ADDR,
            wr_buf, 3, true,
            100000) != 3) return -1;

    return 0;
}

int set_voltage_proj(uint16_t dat) {
    uint8_t wr_buf[3] = {0x10, (dat >> 8) & 0xFF, dat & 0xFF};

    if (i2c_write_timeout_us(
            DAC_PROJ_BUS,
            DAC_PROJ_ADDR,
            wr_buf, 3, true,
            100000) != 3) return -1;

    return 0;
}

int init_dac_proj() {
    uint8_t wr_buf[3] = {0x11, 0x20, 0x00};

    if (i2c_write_timeout_us(
            DAC_PROJ_BUS,
            DAC_PROJ_ADDR,
            wr_buf, 3, true,
            100000) != 3) return -1;

    return 0;
}
