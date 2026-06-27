#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pio_usb.h"
#include "tusb.h"
#include "logger.h"
#include "usb_host.h"
#include "oled_probe.h"
//#include "hardware/i2c.h"

int main(void) {
    logger_init();
    usb_host_init();

    while (true)
    {
        usb_host_task();
        sleep_ms(1);
    }

    return 0;
}