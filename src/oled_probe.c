#include "oled_probe.h"
#include "logger.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

uint8_t dummy = 0;

void oled_probe_task(void)
{
    
    log_printf("oled_probe_task entered\r\n");

    static bool initialized = false;

    if (initialized)
        return;

    log_printf("Before i2c_init\r\n");

    i2c_init(i2c1, 100000);

    log_printf("After i2c_init\r\n");

    gpio_set_function(6, GPIO_FUNC_I2C);
    gpio_set_function(7, GPIO_FUNC_I2C);

    gpio_pull_up(6);
    gpio_pull_up(7);

    log_printf("GPIO configured\r\n");

    for (uint8_t addr = 0x08; addr < 0x78; addr++)
    {
        uint8_t dummy;
        int ret = i2c_read_blocking(i2c1, addr, &dummy, 1, false);

        if (ret >= 0)
        {
            log_printf("Found I2C: 0x%02X\r\n", addr);
        }
    }

    log_printf("I2C scan done\r\n");

    initialized = true;

}


