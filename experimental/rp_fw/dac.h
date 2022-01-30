#include <stdint.h>

void init_dac_i2c();

int set_voltage_caravel(uint16_t dat);
int set_voltage_proj(uint16_t dat);

int init_dac_caravel();
int init_dac_proj();
