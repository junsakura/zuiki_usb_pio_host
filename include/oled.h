#pragma once

#include <stdbool.h>
#include <stdint.h>

// SSD1306 128x64 I2C OLED driver for Raspberry Pi Pico / Pico 2.
// Default wiring:
//   SDA = GP4
//   SCL = GP5
//   I2C = i2c0
//   Address = 0x3C

#define OLED_WIDTH   128
#define OLED_HEIGHT   64
#define OLED_ADDR   0x3C
#define OLED_SDA_PIN  4
#define OLED_SCL_PIN  5

bool oled_init(void);
void oled_clear(void);
void oled_fill(bool on);
void oled_update(void);
void oled_draw_pixel(int x, int y, bool on);
void oled_draw_char(int x, int y, char c);
void oled_draw_string(int x, int y, const char *s);
void oled_printf_line(uint8_t line, const char *fmt, ...);

// Small helper for status screens.
void oled_show_status(const char *line0, const char *line1, const char *line2, const char *line3);
