#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include "tusb.h"

void logger_init(void) {
}

void logger_task(void) {
}

void log_printf(const char *fmt, ...) {
    char buf[256];

    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    if (n <= 0) return;
    if (n > (int)sizeof(buf)) n = sizeof(buf);

    if (tud_cdc_connected()) {
        tud_cdc_write(buf, (uint32_t)n);
        tud_cdc_write_flush();
    }
}

void log_hex(const uint8_t *data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        log_printf("%02X ", data[i]);
    }
}
