#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"

#include "config.h"
#include "dac.h"
#include "datachannel.h"
#include "spiflash.h"

// Chip-select transition counter
uint32_t cs_ctr = 0;
void reset_ctr() { cs_ctr = 0; }

// GPIO interrupt
void gpio_int(unsigned int gpio, uint32_t events) {
    if ((gpio == FLASH_CS_PIN)) {
        cs_ctr++;
    } else if (gpio == CARAVEL_GPIO_PIN) {
        if (events & 0x8) dc_int_rose();
        else if (events & 0x4) dc_int_fell();
    }
}

void setup() {
    //stdio_uart_init_full(uart1, 115200, 8, 9);
    stdio_usb_init();

    init_dac_i2c();

    gpio_init(RSTn_PIN);
    gpio_set_dir(RSTn_PIN, true); // output

    gpio_init(FLASH_CS_PIN);
    gpio_set_dir(FLASH_CS_PIN, false); // input

    gpio_init(CARAVEL_GPIO_PIN);
    gpio_set_dir(CARAVEL_GPIO_PIN, false); // input

    // Enable IRQs
    gpio_set_irq_enabled_with_callback(CARAVEL_GPIO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_int);
    gpio_set_irq_enabled_with_callback(FLASH_CS_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_int);

    init_dac_caravel();
    init_dac_proj();
}

void set_clkdiv(uint32_t div) {
    // Starts with the 48MHz USB clock and divides down
    clock_gpio_init(XCLK_PIN, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_USB, div);
}

int main() {
    sleep_ms(500);
    
    // Configure and set things to sane defaults
    setup();
    set_clkdiv(12); // 4MHz
    set_voltage_caravel(20000);
    set_voltage_proj(20000);

    flash_init();
    flash_deinit();

    sleep_ms(500);

    // Pull chip into reset
    gpio_put(RSTn_PIN, 0);

    // Remote command interface
    while (true) {
        int x = getchar_timeout_us(1000000);
        if (x < 0 || x > 255) continue;

        char cmd = (char) x;
        switch (cmd) {
            case 'R': {
                // Reset (R0/R1)
                int x = getchar_timeout_us(1000000);
                if (x == (int) '0') {
                    gpio_put(RSTn_PIN, 1); // Not-reset
                    putchar_raw('S');

                } else if (x == (int) '1') {
                    gpio_put(RSTn_PIN, 0); // Reset
                    putchar_raw('S');

                } else {
                    putchar_raw('?');
                }
                break;
            }

            case 'T': {
                // Enable for given number of millis, then back into reset
                // (lower-latency than sending two reset commands)

                uint32_t t = 0;
                t |= (getchar_timeout_us(1000000) & 0xFF);
                t |= (getchar_timeout_us(1000000) & 0xFF) << 8;
                t |= (getchar_timeout_us(1000000) & 0xFF) << 16;
                t |= (getchar_timeout_us(1000000) & 0xFF) << 24;

                if (t > 20000) {
                    putchar_raw('?');
                    break;
                }

                putchar_raw('A');

                gpio_put(RSTn_PIN, 0); // Reset
                sleep_ms(10);
                gpio_put(RSTn_PIN, 1); // Not-reset
                sleep_ms(t);
                gpio_put(RSTn_PIN, 0); // Reset

                putchar_raw('S');

                break;
            }

            case 'C': {
                // Set clk div
                int a = getchar_timeout_us(1000000);
                int b = getchar_timeout_us(1000000);
                uint16_t d = ((b & 0xFF) << 8) | (a & 0xFF);

                set_clkdiv(d);
                putchar_raw('S');
                break;
            }

            case 'V': {
                // Set caravel voltage
                int a = getchar_timeout_us(1000000);
                int b = getchar_timeout_us(1000000);
                uint16_t d = ((b & 0xFF) << 8) | (a & 0xFF);

                set_voltage_caravel(d);
                putchar_raw('S');
                break;
            }

            case 'P': {
                // Set project voltage
                int a = getchar_timeout_us(1000000);
                int b = getchar_timeout_us(1000000);
                uint16_t d = ((b & 0xFF) << 8) | (a & 0xFF);

                set_voltage_proj(d);
                putchar_raw('S');
                break;
            }

            case 'E': {
                // Reset counter
                reset_ctr();
                putchar_raw('S');
                break;
            }

            case 'W': {
                // Read counter
                uint32_t c = cs_ctr;
                putchar_raw('S');
                putchar_raw(c & 0xFF);
                putchar_raw((c >> 8) & 0xFF);
                putchar_raw((c >> 16) & 0xFF);
                putchar_raw((c >> 24) & 0xFF);
                break;
            }

            case 'I': {
                // Reset datachannel
                dc_reset();
                putchar_raw('S');
                break;
            }

            case 'O': {
                // Read datachannel buffer length
                uint32_t c = dc_get_len();
                putchar_raw('S');
                putchar_raw(c & 0xFF);
                putchar_raw((c >> 8) & 0xFF);
                putchar_raw((c >> 16) & 0xFF);
                putchar_raw((c >> 24) & 0xFF);
                break;
            }

            case 'U': {
                // Read datachannel buffer
                uint32_t c = dc_get_len();
                putchar_raw('S');
                putchar_raw(c & 0xFF);
                putchar_raw((c >> 8) & 0xFF);
                putchar_raw((c >> 16) & 0xFF);
                putchar_raw((c >> 24) & 0xFF);

                for (uint32_t i = 0; i < c; i++) {
                    putchar_raw(dc_buf[i]);
                }
                break;
            }

            case 'J': {
                // Read flash
                uint32_t addr = 0;
                addr |= (getchar_timeout_us(1000000) & 0xFF);
                addr |= (getchar_timeout_us(1000000) & 0xFF) << 8;
                addr |= (getchar_timeout_us(1000000) & 0xFF) << 16;
                addr |= (getchar_timeout_us(1000000) & 0xFF) << 24;

                uint32_t len = 0;
                len |= (getchar_timeout_us(1000000) & 0xFF);
                len |= (getchar_timeout_us(1000000) & 0xFF) << 8;
                len |= (getchar_timeout_us(1000000) & 0xFF) << 16;
                len |= (getchar_timeout_us(1000000) & 0xFF) << 24;

                if (len > 65535) {
                    putchar_raw('?');
                    break;
                } else {
                    char buf[len];

                    // Reset caravel before accessing flash
                    gpio_put(RSTn_PIN, 0);
                    flash_init();
                    flash_read(addr, buf, len);
                    flash_deinit();

                    putchar_raw('S');
                    for (uint32_t i = 0; i < len; i++) {
                        putchar_raw(buf[i]);
                    }
                }
                
                break;
            }

            case 'K': {
                // Write flash
                uint32_t addr = 0;
                addr |= (getchar_timeout_us(1000000) & 0xFF);
                addr |= (getchar_timeout_us(1000000) & 0xFF) << 8;
                addr |= (getchar_timeout_us(1000000) & 0xFF) << 16;
                addr |= (getchar_timeout_us(1000000) & 0xFF) << 24;

                char buf[4096];

                for (int i = 0; i < 4096; i++) {
                    buf[i] = getchar_timeout_us(1000000) & 0xFF;
                }

                putchar_raw('A');

                // Reset caravel before accessing flash
                gpio_put(RSTn_PIN, 0);
                flash_init();
                int res = flash_write_sector(addr, buf);
                int res2 = flash_verify_sector(addr, buf);
                flash_deinit();

                if (res == 0 && res2 == 0) putchar_raw('S');
                else putchar_raw('X');
                
                break;
            }

            default: {
                putchar_raw('?');
                break;
            }
        }
    }

    while (1);
    return 0;
}
