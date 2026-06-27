#pragma once

#include <stdint.h>

void logger_init(void);
void logger_task(void);

void log_printf(const char *fmt, ...);
void log_hex(const uint8_t *data, uint16_t len);
