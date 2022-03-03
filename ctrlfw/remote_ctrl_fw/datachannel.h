#include <stdint.h>

#define DC_BUFSIZE 2048

extern char dc_buf[DC_BUFSIZE];

uint32_t dc_get_len();

void dc_reset();
void dc_int_rose();
void dc_int_fell();
