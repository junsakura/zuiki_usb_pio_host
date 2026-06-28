#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pio_usb.h"
#include "tusb.h"

#include "logger.h"
#include "usb_host.h"
#include "oled.h"

#include "usb_status.h"

static void core1_oled_main(void)
{
    bool oled_ready = false;
    uint32_t start_ms = to_ms_since_boot(get_absolute_time());
    uint32_t last_oled = 0;

    while (true) {
        uint32_t now = to_ms_since_boot(get_absolute_time());

        if (!oled_ready && now - start_ms > 3000) {
            oled_ready = oled_init();

            if (oled_ready) {
                oled_fill(true);
                oled_update();
                sleep_ms(300);

                oled_clear();
                oled_printf_line(0, "OLED READY");
                oled_printf_line(1, "USB HOST OK");
                oled_update();
            }
        }

        if (oled_ready && now - last_oled > 500) {
            last_oled = now;

            oled_clear();

            oled_printf_line(0, "USB %s",
                g_usb_status.usb_connected ? "CONNECTED" : "WAIT");

            oled_printf_line(1, "HID %s",
                g_usb_status.hid_connected ? "CONNECTED" : "WAIT");

            oled_printf_line(2, "VID %04X", g_usb_status.vid);
            oled_printf_line(3, "PID %04X", g_usb_status.pid);
            oled_printf_line(4, "LEN %d", g_usb_status.report_len);

            oled_printf_line(5, "%02X %02X %02X %02X",
                g_usb_status.report[0],
                g_usb_status.report[1],
                g_usb_status.report[2],
                g_usb_status.report[3]);

            oled_printf_line(6, "%02X %02X %02X %02X",
                g_usb_status.report[4],
                g_usb_status.report[5],
                g_usb_status.report[6],
                g_usb_status.report[7]);

            oled_update();
        }

        sleep_ms(10);
    }
}

int main(void) {
    bool oled_ready = false;
    uint32_t start_ms = to_ms_since_boot(get_absolute_time());
    uint32_t last_oled = 0;

    logger_init();
    usb_host_init();
	multicore_launch_core1(core1_oled_main);
	
    while (true) {
		usb_host_task();
    }


    return 0;
}
