#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pio_usb.h"
#include "tusb.h"

#include "logger.h"
#include "usb_host.h"
#include "oled.h"

int main(void) {
	bool oled_ready = false;
	bool oled_test_done = false;

	uint32_t start_ms = to_ms_since_boot(get_absolute_time());
	uint32_t last_oled_ms = 0;

	logger_init();
    usb_host_init();
	
    while (true) {
    	usb_host_task();

    	uint32_t now = to_ms_since_boot(get_absolute_time());

    	if (!oled_ready && now - start_ms > 3000) {
    	    oled_ready = oled_init();

	        if (oled_ready) {
            	oled_fill(true);
           		oled_update();
            	sleep_ms(500);

            	oled_clear();
            	oled_draw_string(0, 0, "OLED TEST");
            	oled_draw_string(0, 16, "I2C 0X3C");
            	oled_draw_string(0, 32, "USB HOST OK");
           		oled_update();

            	oled_test_done = true;
        	}
    	}

    	sleep_ms(1);
    }


    return 0;
}
