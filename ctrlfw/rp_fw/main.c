#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "config.h"
#include "dac.h"
#include "hardware/clocks.h"

uint32_t ctr = 0;
void reset_ctr() { ctr = 0; }

void run_sweep() {
    // Sweep
    //for (uint16_t i = 20500; i < 21200; i += 5) {
    for (uint16_t i = 13500; i < 26000; i += 40) {
        gpio_put(RSTn_PIN, 0);
        sleep_ms(10);
        reset_ctr();
        printf("cfg %d %d %d -> ", i, set_voltage_caravel(i), set_voltage_proj(i));
        gpio_put(RSTn_PIN, 1);
        sleep_ms(20);
        //gpio_put(RSTn_PIN, 0);
        printf("%d\n", ctr);
    }
}

int run_repeated_tests(int repeats) {
    int successes = 0;
    for (int x = 0; x < repeats; x++) {
        gpio_put(RSTn_PIN, 0);
        reset_ctr();
        gpio_put(RSTn_PIN, 1);
        sleep_ms(30);
        gpio_put(RSTn_PIN, 0);
        if (ctr > 1200 && ctr < 1400) successes++;
        sleep_ms(30);
    }

    return successes;
}

void run_repeated() {
    int repeats = 20;

    // Sweep
    for (uint16_t i = 20500; i < 21200; i += 10) {
        printf("cfg %d %d %d -> ", i, set_voltage_caravel(i), set_voltage_proj(i));
        int successes = run_repeated_tests(repeats);
        printf("%d\n", successes);
    }
}

char outputbuf[256];
uint8_t cur_char = 0;
int bit_ctr = 0;
int char_ctr = 0;
uint64_t risetime = 0;

void reset_outputbuf() {
    cur_char = 0;
    bit_ctr = 0;
    char_ctr = 0;
    risetime = 0;
}

void caravel_gpio(unsigned int gpio, uint32_t events) {
    if ((gpio == FLASH_CS_PIN)) {
        ctr++;

    } else if (gpio == CARAVEL_GPIO_PIN) {

        if (events & 0x8) { // Rise
            risetime = time_us_64();

        } else if (events & 0x4) {
            uint64_t time = time_us_64() - risetime;

            if (time > 500) {
                cur_char |= (0x1 << bit_ctr);
            }

            bit_ctr += 1;
            if (bit_ctr == 8) {
                bit_ctr = 0;
                outputbuf[char_ctr] = cur_char;
                char_ctr += 1;
                cur_char = 0;
            }
        }

    }
}

void hexprintln(char *buf, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02x", buf[i]);
    }
    printf("\n");
}

void run_sweep2() {
    // Sweep
    //for (uint16_t i = 20500; i < 21200; i += 5) {
    //for (uint16_t i = 19000; i < 22000; i += 10) {
    for (uint16_t i = 16700; i < 22900; i += 200) {
        gpio_put(RSTn_PIN, 0);
        sleep_ms(10);
        reset_ctr();
        printf("cfg %d %d %d -> ", i, set_voltage_caravel(20500), set_voltage_proj(i));
        reset_outputbuf();
        gpio_put(RSTn_PIN, 1);
        sleep_ms(500);
        //gpio_put(RSTn_PIN, 0);
        outputbuf[char_ctr] = 0;
        //printf("readout %s\n", outputbuf);
        printf("%d %d\n", ctr, char_ctr);
        printf("readout  = ");
        //hexprintln(outputbuf, 16);
        hexprintln(outputbuf, 32);
        printf("expected = 41812dc60561798dc0cc6e574b641893ecf4186d4097283af4a6cff3eddaa4a0\n");
    }
}

int main() {
    stdio_usb_init();
    stdio_uart_init_full(uart1, 115200, 8, 9);

    printf("Gmorn\n");
    sleep_ms(1000);

    printf("SWEEP\n");

    init_dac_i2c();

    gpio_init(RSTn_PIN);
    gpio_set_dir(RSTn_PIN, true); // output

    gpio_init(FLASH_CS_PIN);
    gpio_set_dir(FLASH_CS_PIN, false); // input
    //gpio_set_irq_enabled_with_callback(FLASH_CS_PIN, GPIO_IRQ_EDGE_FALL, true, &ctr_interrupt);

    gpio_init(CARAVEL_GPIO_PIN);
    gpio_set_dir(CARAVEL_GPIO_PIN, false); // input

    // apparently this handles all IRQs
    gpio_set_irq_enabled_with_callback(CARAVEL_GPIO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &caravel_gpio);
    gpio_set_irq_enabled_with_callback(FLASH_CS_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &caravel_gpio);

    // clock output 500khz
    clock_gpio_init(XCLK_PIN, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_USB, 12);

    printf("Setup Caravel DAC %d\n", init_dac_caravel());
    printf("Setup Project DAC %d\n", init_dac_proj());
    sleep_ms(500);

    //printf("cfg %d %d %d -> ", 21050, set_voltage_caravel(21050), set_voltage_proj(21050));
    //int successes = run_repeated_tests(100);
    //printf("%d\n", successes);

    run_sweep2();
    //run_sweep();

    gpio_put(RSTn_PIN, 0);
    //run_repeated();

    while (1);

    return 0;
}
