#include "oled.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

static uint8_t oled_buf[OLED_WIDTH * OLED_HEIGHT / 8];
static bool oled_ready = false;

static bool oled_write_cmd(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    return i2c_write_blocking(i2c0, OLED_ADDR, buf, 2, false) == 2;
}

static bool oled_write_cmd2(uint8_t cmd, uint8_t arg) {
    uint8_t buf[3] = {0x00, cmd, arg};
    return i2c_write_blocking(i2c0, OLED_ADDR, buf, 3, false) == 3;
}

static const uint8_t *font5x7(char c) {
    // 5 columns, bit0 = top pixel. Characters not listed become space.
    static const uint8_t space[5] = {0x00,0x00,0x00,0x00,0x00};

    if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');

    switch (c) {
    case ' ': return space;
    case '!': { static const uint8_t f[5]={0x00,0x00,0x5F,0x00,0x00}; return f; }
    case '.': { static const uint8_t f[5]={0x00,0x60,0x60,0x00,0x00}; return f; }
    case ':': { static const uint8_t f[5]={0x00,0x36,0x36,0x00,0x00}; return f; }
    case '-': { static const uint8_t f[5]={0x08,0x08,0x08,0x08,0x08}; return f; }
    case '_': { static const uint8_t f[5]={0x40,0x40,0x40,0x40,0x40}; return f; }
    case '/': { static const uint8_t f[5]={0x20,0x10,0x08,0x04,0x02}; return f; }
    case '0': { static const uint8_t f[5]={0x3E,0x51,0x49,0x45,0x3E}; return f; }
    case '1': { static const uint8_t f[5]={0x00,0x42,0x7F,0x40,0x00}; return f; }
    case '2': { static const uint8_t f[5]={0x42,0x61,0x51,0x49,0x46}; return f; }
    case '3': { static const uint8_t f[5]={0x21,0x41,0x45,0x4B,0x31}; return f; }
    case '4': { static const uint8_t f[5]={0x18,0x14,0x12,0x7F,0x10}; return f; }
    case '5': { static const uint8_t f[5]={0x27,0x45,0x45,0x45,0x39}; return f; }
    case '6': { static const uint8_t f[5]={0x3C,0x4A,0x49,0x49,0x30}; return f; }
    case '7': { static const uint8_t f[5]={0x01,0x71,0x09,0x05,0x03}; return f; }
    case '8': { static const uint8_t f[5]={0x36,0x49,0x49,0x49,0x36}; return f; }
    case '9': { static const uint8_t f[5]={0x06,0x49,0x49,0x29,0x1E}; return f; }
    case 'A': { static const uint8_t f[5]={0x7E,0x11,0x11,0x11,0x7E}; return f; }
    case 'B': { static const uint8_t f[5]={0x7F,0x49,0x49,0x49,0x36}; return f; }
    case 'C': { static const uint8_t f[5]={0x3E,0x41,0x41,0x41,0x22}; return f; }
    case 'D': { static const uint8_t f[5]={0x7F,0x41,0x41,0x22,0x1C}; return f; }
    case 'E': { static const uint8_t f[5]={0x7F,0x49,0x49,0x49,0x41}; return f; }
    case 'F': { static const uint8_t f[5]={0x7F,0x09,0x09,0x09,0x01}; return f; }
    case 'G': { static const uint8_t f[5]={0x3E,0x41,0x49,0x49,0x7A}; return f; }
    case 'H': { static const uint8_t f[5]={0x7F,0x08,0x08,0x08,0x7F}; return f; }
    case 'I': { static const uint8_t f[5]={0x00,0x41,0x7F,0x41,0x00}; return f; }
    case 'J': { static const uint8_t f[5]={0x20,0x40,0x41,0x3F,0x01}; return f; }
    case 'K': { static const uint8_t f[5]={0x7F,0x08,0x14,0x22,0x41}; return f; }
    case 'L': { static const uint8_t f[5]={0x7F,0x40,0x40,0x40,0x40}; return f; }
    case 'M': { static const uint8_t f[5]={0x7F,0x02,0x0C,0x02,0x7F}; return f; }
    case 'N': { static const uint8_t f[5]={0x7F,0x04,0x08,0x10,0x7F}; return f; }
    case 'O': { static const uint8_t f[5]={0x3E,0x41,0x41,0x41,0x3E}; return f; }
    case 'P': { static const uint8_t f[5]={0x7F,0x09,0x09,0x09,0x06}; return f; }
    case 'Q': { static const uint8_t f[5]={0x3E,0x41,0x51,0x21,0x5E}; return f; }
    case 'R': { static const uint8_t f[5]={0x7F,0x09,0x19,0x29,0x46}; return f; }
    case 'S': { static const uint8_t f[5]={0x46,0x49,0x49,0x49,0x31}; return f; }
    case 'T': { static const uint8_t f[5]={0x01,0x01,0x7F,0x01,0x01}; return f; }
    case 'U': { static const uint8_t f[5]={0x3F,0x40,0x40,0x40,0x3F}; return f; }
    case 'V': { static const uint8_t f[5]={0x1F,0x20,0x40,0x20,0x1F}; return f; }
    case 'W': { static const uint8_t f[5]={0x3F,0x40,0x38,0x40,0x3F}; return f; }
    case 'X': { static const uint8_t f[5]={0x63,0x14,0x08,0x14,0x63}; return f; }
    case 'Y': { static const uint8_t f[5]={0x07,0x08,0x70,0x08,0x07}; return f; }
    case 'Z': { static const uint8_t f[5]={0x61,0x51,0x49,0x45,0x43}; return f; }
    default: return space;
    }
}

bool oled_init(void) {
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA_PIN);
    gpio_pull_up(OLED_SCL_PIN);

    sleep_ms(100);

    //Probe address first.
//    uint8_t probe = 0;
//    if (i2c_write_blocking(i2c0, OLED_ADDR, &probe, 1, false) < 0) {
//        oled_ready = false;
//        return false;
//    }

    oled_write_cmd(0xAE);       // display off
    oled_write_cmd2(0xD5, 0x80); // clock divide
    oled_write_cmd2(0xA8, 0x3F); // multiplex 1/64
    oled_write_cmd2(0xD3, 0x00); // display offset
    oled_write_cmd(0x40);       // start line
    oled_write_cmd2(0x8D, 0x14); // charge pump on
    oled_write_cmd2(0x20, 0x00); // horizontal addressing
    oled_write_cmd(0xA1);       // segment remap
    oled_write_cmd(0xC8);       // COM scan direction
    oled_write_cmd2(0xDA, 0x12); // COM pins
    oled_write_cmd2(0x81, 0x7F); // contrast
    oled_write_cmd2(0xD9, 0xF1); // pre-charge
    oled_write_cmd2(0xDB, 0x40); // vcomh
    oled_write_cmd(0xA4);       // resume RAM display
    oled_write_cmd(0xA6);       // normal display
    oled_write_cmd(0xAF);       // display on

    oled_ready = true;
    oled_clear();
    oled_update();
    return true;
}

void oled_clear(void) {
    memset(oled_buf, 0x00, sizeof(oled_buf));
}

void oled_fill(bool on) {
    memset(oled_buf, on ? 0xFF : 0x00, sizeof(oled_buf));
}

void oled_update(void) {
    if (!oled_ready) return;

    oled_write_cmd2(0x21, 0);                 // column address
    oled_write_cmd( OLED_WIDTH - 1 );
    oled_write_cmd2(0x22, 0);                 // page address
    oled_write_cmd( (OLED_HEIGHT / 8) - 1 );

    // Send framebuffer in chunks. First byte is SSD1306 control byte for data.
    uint8_t packet[17];
    packet[0] = 0x40;
    for (int i = 0; i < (int)sizeof(oled_buf); i += 16) {
        memcpy(&packet[1], &oled_buf[i], 16);
        i2c_write_blocking(i2c0, OLED_ADDR, packet, 17, false);
    }
}

void oled_draw_pixel(int x, int y, bool on) {
    if (x < 0 || x >= OLED_WIDTH || y < 0 || y >= OLED_HEIGHT) return;

    uint16_t index = (uint16_t)x + (uint16_t)(y / 8) * OLED_WIDTH;
    uint8_t mask = (uint8_t)(1u << (y & 7));

    if (on) oled_buf[index] |= mask;
    else    oled_buf[index] &= (uint8_t)~mask;
}

void oled_draw_char(int x, int y, char c) {
    const uint8_t *glyph = font5x7(c);

    for (int col = 0; col < 5; col++) {
        uint8_t bits = glyph[col];
        for (int row = 0; row < 7; row++) {
            oled_draw_pixel(x + col, y + row, (bits & (1u << row)) != 0);
        }
    }
    // 1px spacing
    for (int row = 0; row < 7; row++) {
        oled_draw_pixel(x + 5, y + row, false);
    }
}

void oled_draw_string(int x, int y, const char *s) {
    while (*s && x <= OLED_WIDTH - 6) {
        oled_draw_char(x, y, *s++);
        x += 6;
    }
}

void oled_printf_line(uint8_t line, const char *fmt, ...) {
    if (line > 7) return;

    char text[24];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(text, sizeof(text), fmt, ap);
    va_end(ap);

    // Clear one 8-pixel-high text line.
    int y = line * 8;
    for (int yy = y; yy < y + 8; yy++) {
        for (int x = 0; x < OLED_WIDTH; x++) {
            oled_draw_pixel(x, yy, false);
        }
    }

    oled_draw_string(0, y, text);
}

void oled_show_status(const char *line0, const char *line1, const char *line2, const char *line3) {
    oled_clear();
    if (line0) oled_draw_string(0,  0, line0);
    if (line1) oled_draw_string(0, 16, line1);
    if (line2) oled_draw_string(0, 32, line2);
    if (line3) oled_draw_string(0, 48, line3);
    oled_update();
	
	if (oled_ready) {
    	oled_fill(true);
    	oled_update();
    	sleep_ms(1000);

    	oled_clear();
    	oled_draw_string(0, 0, "OLED TEST");
    	oled_draw_string(0, 16, "I2C 0X3C");
    	oled_update();
	}
	
}
