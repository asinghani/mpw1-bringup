#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "datachannel.h"

char dc_buf[DC_BUFSIZE];

uint8_t cur_char = 0;
uint32_t bit_ctr = 0;
uint32_t char_ctr = 0;
uint64_t risetime = 0;

uint64_t thresh = 0;
uint64_t threshctr = 0;
uint64_t minthresh = 0;
uint64_t maxthresh = 0;

uint32_t dc_get_len() {
    return char_ctr;
}

void dc_reset() {
    cur_char = 0;
    bit_ctr = 0;
    char_ctr = 0;
    risetime = 0;

    thresh = 0;
    threshctr = 0;
    minthresh = 0;
    maxthresh = 0;

    memset(dc_buf, 0, DC_BUFSIZE);
}


void dc_int_rose() {
    risetime = time_us_64();
}

void dc_int_fell() {
    // Pulsed data transmission, short pulse = 0,
    // long pulse = 1 (similar to WS2812)
    uint64_t pulsetime = time_us_64() - risetime;

    if (threshctr < 8) {
        // First 8 bits after startup are a calibration pattern 
        // for clock speed: 0,1,0,1,0,1,0,1 (0xAA, sent LSB-first)
        if ((threshctr % 2) == 0) minthresh += pulsetime;
        else maxthresh += pulsetime;

        threshctr++;

        if (threshctr == 8) {
            thresh = (minthresh + maxthresh) / 8;
        }

    } else {
        // After the calibration, use the computed threshold
        if (pulsetime > thresh) {
            cur_char |= (0x1 << bit_ctr);
        }

        bit_ctr += 1;
        if (bit_ctr == 8) {
            bit_ctr = 0;
            dc_buf[char_ctr] = cur_char;
            char_ctr += 1;
            cur_char = 0;
        }
    }
}

