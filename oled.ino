#include <application.h>

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// Hardware SPI
#define OLED_DC D3
#define OLED_CS D4
#define OLED_RESET D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

/**
 * Modes
 * 
 * Boot screen
 * Clock
 * Big scroll
 * Internet draw
 **/
#define MODE_BOOT 1
#define MODE_CLOCK 2
#define MODE_SCROLL 3
#define MODE_DRAW 4

#define BOUNCE_AMOUNT 4

#define DRAW_TIMEOUT 60000
#define BOOTSCREEN_TIMEOUT 5000
#define SCROLL_TIMEOUT 1000

// Big numbers
#define NUMBERS_WIDTH 24
#define NUMBERS_HEIGHT 56
static uint8_t NUMBERS_BITS[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xf6, 0xff, 0x7f, 0xf6, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x00,
    0xe0, 0xff, 0x00, 0x70, 0xff, 0x01, 0x70, 0xff, 0x01, 0x70, 0xff, 0x01,
    0xf0, 0xff, 0x01, 0xf0, 0xff, 0x01, 0xe0, 0xff, 0x01, 0xe0, 0xff, 0x01,
    0xe0, 0xff, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01,
    0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01,
    0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01,
    0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01,
    0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01,
    0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01,
    0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01,
    0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01,
    0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01,
    0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfc, 0x00,
    0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xf6, 0xff, 0x7f, 0xf6, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfc, 0xc1, 0x7f,
    0xfc, 0xc1, 0x7f, 0xfc, 0xc1, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0xfc, 0xff, 0x7f,
    0xfc, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x3f,
    0xfe, 0xff, 0x3f, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00,
    0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00,
    0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0xff, 0x3f,
    0xfe, 0xff, 0x3f, 0xfe, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xf6, 0xff, 0x7f, 0xf6, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xe3, 0x7f, 0xfe, 0xe3, 0x7f, 0xfc, 0xc1, 0x7f,
    0xfc, 0xc1, 0x7f, 0xfc, 0xc1, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xe0, 0x7f, 0x00, 0xe0, 0x7f, 0x00, 0xe0, 0x7f, 0xf0, 0xff, 0x3f,
    0xf0, 0xff, 0x3f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f,
    0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf0, 0xff, 0x3f,
    0xf0, 0xff, 0x3f, 0x00, 0xe0, 0x7f, 0x00, 0xe0, 0x7f, 0x00, 0xe0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0xfc, 0xc1, 0x7f, 0xfc, 0xc1, 0x7f,
    0xfc, 0xc1, 0x7f, 0xfe, 0xe3, 0x7f, 0xfe, 0xe3, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x81, 0x3f,
    0xfc, 0x81, 0x3f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xf6, 0xc3, 0x7f, 0xf6, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x7f,
    0xfc, 0xff, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0x80, 0x3f,
    0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xf6, 0xff, 0x7f, 0xf6, 0xff, 0x7f, 0xfe, 0xff, 0x3f, 0xfe, 0xff, 0x3f,
    0xfe, 0xff, 0x3f, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00,
    0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00,
    0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0xff, 0x3f,
    0xfe, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x7f,
    0xfc, 0xff, 0x7f, 0x00, 0xe0, 0x7f, 0x00, 0xe0, 0x7f, 0x00, 0xe0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0xfc, 0xc1, 0x7f, 0xfc, 0xc1, 0x7f,
    0xfc, 0xc1, 0x7f, 0xfe, 0xe3, 0x7f, 0xfe, 0xe3, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xf6, 0xff, 0x7f, 0xf6, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xc7, 0x7f, 0xfe, 0xc7, 0x7f, 0xfe, 0x83, 0x3f,
    0xfe, 0x83, 0x3f, 0xfe, 0x83, 0x3f, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00,
    0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0x03, 0x00, 0xfe, 0xff, 0x3f,
    0xfe, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xf6, 0xff, 0x7f, 0xf6, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfc, 0xc1, 0x7f,
    0xfc, 0xc1, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0x80, 0x3f,
    0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x1f,
    0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x3f, 0xfc, 0xff, 0x3f, 0xfc, 0xff, 0x3f,
    0xf6, 0xff, 0x7f, 0xf6, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0x81, 0x7f,
    0xfe, 0x81, 0x7f, 0xfe, 0x81, 0x7f, 0xfe, 0x81, 0x7f, 0xfe, 0x81, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f,
    0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0x81, 0x7f, 0xfe, 0x81, 0x7f, 0xfe, 0x81, 0x7f, 0xfe, 0x81, 0x7f,
    0xfe, 0x81, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfc, 0xff, 0x3f, 0xfc, 0xff, 0x3f, 0xfc, 0xff, 0x3f, 0xf8, 0xff, 0x1f,
    0xf8, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xf6, 0xff, 0x7f, 0xf6, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f,
    0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xc3, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x7f,
    0xfc, 0xff, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f,
    0x00, 0xc0, 0x7f, 0x00, 0xc0, 0x7f, 0xfc, 0xc1, 0x7f, 0xfc, 0xc1, 0x7f,
    0xfc, 0xc1, 0x7f, 0xfe, 0xe3, 0x7f, 0xfe, 0xe3, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f,
    0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f,
    0xfc, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define COLON_WIDTH 7
#define COLON_HEIGHT 27
static const uint8_t COLON_BITS[] = {
    0x3e, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x7f, 0x7f, 0x7f,
    0x7f, 0x7f, 0x3e};

// Kickstart boot screen
#define BOOTSCREEN_WIDTH 56
#define BOOTSCREEN_HEIGHT 53
static const uint8_t BOOTSCREEN_BITS[] = {
    0x00, 0xfe, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0xfe, 0x1f, 0x00, 0x00,
    0xfe, 0x00, 0x00, 0xfe, 0x1f, 0x00, 0xf8, 0xfe, 0x01, 0x00, 0xfe, 0x1f,
    0x00, 0xf8, 0xfe, 0x03, 0x00, 0xfe, 0x1f, 0x00, 0xf8, 0xfe, 0x07, 0x00,
    0xfe, 0x1f, 0x00, 0xf8, 0xfe, 0x0f, 0x00, 0xfe, 0x1f, 0x00, 0xf8, 0xfe,
    0x0f, 0x00, 0xfe, 0x1f, 0x00, 0xf8, 0xfe, 0x0f, 0x00, 0xfe, 0x1f, 0x00,
    0x00, 0xfe, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x0f,
    0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0xff,
    0xff, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xfe, 0xff,
    0xff, 0xff, 0xff, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00,
    0xfe, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xff, 0xfb, 0xff, 0xff, 0xff,
    0x0f, 0x40, 0xfe, 0x61, 0x00, 0x00, 0xc0, 0x0f, 0x40, 0xfe, 0x61, 0x00,
    0x00, 0xc0, 0x0f, 0x20, 0xfe, 0x63, 0x00, 0x00, 0xc0, 0x0f, 0x20, 0xfe,
    0x7c, 0x20, 0x10, 0xc0, 0x0f, 0x10, 0x7e, 0x40, 0x36, 0x93, 0xc1, 0x0f,
    0x10, 0x3e, 0x40, 0x00, 0x00, 0xc0, 0x0f, 0xc4, 0x06, 0x10, 0x00, 0x00,
    0xc0, 0x0f, 0x00, 0x03, 0x10, 0x54, 0x55, 0xc0, 0x0f, 0x82, 0x00, 0x04,
    0x54, 0x44, 0xc0, 0x0f, 0x41, 0x00, 0x02, 0x10, 0x54, 0xc0, 0x0f, 0x31,
    0x00, 0x01, 0x00, 0x00, 0xc0, 0x0f, 0x01, 0x00, 0x41, 0x9d, 0x54, 0xc5,
    0x0f, 0x01, 0x00, 0x41, 0x50, 0x32, 0xc3, 0x0f, 0x01, 0x00, 0x01, 0x6e,
    0x13, 0xc1, 0x0f, 0x01, 0x00, 0x02, 0x00, 0x00, 0xc0, 0x0f, 0x01, 0x00,
    0xfe, 0xff, 0xff, 0xff, 0x0f, 0x01, 0x00, 0x1a, 0x46, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x26, 0x43, 0x00, 0x00, 0x00, 0x01, 0x00, 0xc2, 0x21, 0x00,
    0x00, 0x00, 0x01, 0x00, 0xfe, 0x23, 0x00, 0x00, 0x00, 0x01, 0x00, 0xc2,
    0x28, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x11, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x02, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x06, 0x00, 0x00,
    0x00, 0x01, 0xf0, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x10,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x10,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00};

// Netset big bmp
#define BITMAP_WIDTH 38  // Nbr chars
static const uint8_t BITMAP_BITS[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
    0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xe0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xe0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xc0, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
    0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0x00, 0x00,
    0x80, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x07, 0x00, 0x00,
    0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x0f, 0x00, 0x00,
    0xff, 0xff, 0x3f, 0xc0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff,
    0x01, 0x00, 0xf8, 0xff, 0xff, 0xff, 0x1f, 0x00, 0x00, 0xfe, 0xff, 0x3f,
    0xf0, 0x07, 0x00, 0x00, 0xfe, 0xff, 0x07, 0x00, 0xc0, 0xff, 0xff, 0xff,
    0xff, 0x00, 0xff, 0xff, 0x3f, 0xf0, 0xff, 0xff, 0x01, 0x00, 0x00, 0xf0,
    0xff, 0xff, 0x07, 0x00, 0xf8, 0xff, 0xff, 0xff, 0x1f, 0x00, 0xc0, 0xff,
    0xff, 0xff, 0xf9, 0x07, 0x00, 0xc0, 0xff, 0xff, 0x3f, 0x00, 0xc0, 0xff,
    0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x3f, 0xf8, 0xff, 0xff, 0x03, 0x00,
    0x00, 0xfc, 0xff, 0xff, 0x1f, 0x00, 0xf8, 0xff, 0xff, 0xff, 0x1f, 0x00,
    0xe0, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0xe0, 0xff, 0xff, 0x7f, 0x00,
    0xc0, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff,
    0x03, 0x00, 0x00, 0xff, 0xff, 0xff, 0x3f, 0x00, 0xf8, 0xff, 0xff, 0xff,
    0x1f, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0xf8, 0xff, 0xff,
    0xff, 0x01, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x3f, 0xfe,
    0xff, 0xff, 0x07, 0x00, 0x80, 0xff, 0xff, 0xff, 0x7f, 0x00, 0xf8, 0xff,
    0xff, 0xff, 0x1f, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0xfc,
    0xff, 0xff, 0xff, 0x03, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
    0x3f, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x00,
    0xf8, 0xff, 0xff, 0xff, 0x1f, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x07,
    0x00, 0xfe, 0xff, 0xff, 0xff, 0x07, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xe0, 0xff, 0x1f, 0xf8,
    0xff, 0x01, 0xf8, 0xff, 0xff, 0xff, 0x1f, 0x00, 0xfe, 0xff, 0x07, 0xf0,
    0xff, 0x07, 0x00, 0xff, 0x7f, 0xc0, 0xff, 0x0f, 0xc0, 0xff, 0xff, 0xff,
    0xff, 0x00, 0xe0, 0xff, 0xff, 0x0f, 0xfc, 0xff, 0x0f, 0x00, 0xf0, 0xff,
    0x07, 0xf0, 0xff, 0x03, 0xf0, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xff, 0xff,
    0x01, 0xc0, 0xff, 0x07, 0x80, 0xff, 0x3f, 0x80, 0xff, 0x1f, 0x80, 0xff,
    0xff, 0xff, 0x7f, 0x00, 0x80, 0xff, 0xff, 0x03, 0xf0, 0xff, 0x1f, 0x00,
    0xf0, 0xff, 0x03, 0xe0, 0xff, 0x07, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0xff, 0x07, 0xc0, 0xff, 0x1f, 0x00, 0xff, 0x3f,
    0x00, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff, 0xff, 0x01, 0xe0, 0xff,
    0x1f, 0x00, 0xf8, 0xff, 0x03, 0xc0, 0xff, 0x07, 0x00, 0xfc, 0xff, 0x01,
    0x00, 0x80, 0xff, 0xff, 0x00, 0x00, 0xfe, 0x07, 0xc0, 0xff, 0x0f, 0x00,
    0xfe, 0x3f, 0x00, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff, 0xff, 0x01,
    0xe0, 0xff, 0x1f, 0x00, 0xfc, 0xff, 0x01, 0xc0, 0xff, 0x0f, 0x00, 0xfc,
    0xff, 0x01, 0x00, 0x80, 0xff, 0xff, 0x00, 0x00, 0xfc, 0x07, 0xe0, 0xff,
    0x0f, 0x00, 0xfe, 0x7f, 0x00, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff,
    0xff, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xfc, 0xff, 0x01, 0x80, 0xff, 0x1f,
    0x00, 0xfc, 0xff, 0x01, 0x00, 0x80, 0xff, 0xff, 0x01, 0x00, 0xf8, 0x07,
    0xe0, 0xff, 0x0f, 0x00, 0xfe, 0x7f, 0x00, 0xe0, 0xff, 0x0f, 0x00, 0x00,
    0x80, 0xff, 0xff, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xfe, 0xff, 0x01, 0x80,
    0xff, 0x1f, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x80, 0xff, 0xff, 0x03, 0x00,
    0xf8, 0x07, 0xf0, 0xff, 0x07, 0x00, 0xfc, 0xff, 0x00, 0xe0, 0xff, 0x0f,
    0x00, 0x00, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xfe, 0xff,
    0x01, 0x80, 0xff, 0x1f, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x80, 0xff, 0xff,
    0x1f, 0x00, 0xf0, 0x03, 0xf0, 0xff, 0x07, 0x00, 0xfc, 0xff, 0x00, 0xe0,
    0xff, 0x0f, 0x00, 0x00, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
    0xfe, 0xff, 0x00, 0x80, 0xff, 0x3f, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x80,
    0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0xf8, 0xff, 0x07, 0x00, 0xfc, 0xff,
    0x01, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff,
    0x1f, 0x00, 0xff, 0xff, 0x00, 0x80, 0xff, 0x3f, 0x00, 0xfc, 0xff, 0x01,
    0x00, 0x80, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0xf8, 0xff, 0x07, 0x00,
    0xfc, 0xff, 0x01, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff, 0x7f, 0x00,
    0xc0, 0xff, 0x1f, 0x00, 0xff, 0xff, 0x01, 0xc0, 0xff, 0x3f, 0x00, 0xfc,
    0xff, 0x01, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xf8, 0xff,
    0x0f, 0x00, 0xfe, 0xff, 0x01, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff,
    0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f,
    0x00, 0xfc, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00,
    0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xe0, 0xff, 0x0f, 0x00, 0x00,
    0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x7f, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0x3f, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xe0, 0xff, 0x0f,
    0x00, 0x00, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x00, 0xfe, 0xff,
    0xff, 0xff, 0xff, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xe0,
    0xff, 0x0f, 0x00, 0x00, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x00,
    0xfe, 0xff, 0xff, 0xff, 0xff, 0x01, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x03, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff,
    0x1f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0xfc, 0xff, 0x01,
    0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x03, 0xfc, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x03, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff, 0x7f, 0x00,
    0xc0, 0xff, 0x1f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0xfc,
    0xff, 0x01, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x07, 0xfc, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x03, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff,
    0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f,
    0x00, 0xfc, 0xff, 0x01, 0xfe, 0x01, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x0f,
    0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xe0, 0xff, 0x0f, 0xf0, 0x07,
    0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x7f, 0x00, 0xfc, 0xff, 0x01, 0xfe, 0x01, 0x80, 0xff, 0xff, 0xff,
    0xff, 0x0f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xe0, 0xff, 0x0f,
    0xf0, 0x07, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x01, 0xfe, 0x01, 0x00, 0xfc,
    0xff, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0xe0,
    0xff, 0x0f, 0xf0, 0x07, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x01, 0xfe, 0x01,
    0x00, 0xe0, 0xff, 0xff, 0xff, 0x1f, 0xf8, 0xff, 0x07, 0x00, 0x00, 0x00,
    0x00, 0xe0, 0xff, 0x0f, 0xf0, 0x07, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff,
    0x1f, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x01,
    0xfe, 0x01, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x1f, 0xf8, 0xff, 0x07, 0x00,
    0x00, 0x00, 0x00, 0xe0, 0xff, 0x0f, 0xf0, 0x07, 0x80, 0xff, 0x7f, 0x00,
    0xc0, 0xff, 0x1f, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc,
    0xff, 0x01, 0xfe, 0x01, 0x7f, 0x00, 0xe0, 0xff, 0xff, 0x1f, 0xf8, 0xff,
    0x07, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x0f, 0xf0, 0x07, 0x80, 0xff,
    0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xff, 0xff, 0x01, 0x00, 0xf0, 0x3f,
    0x00, 0xfc, 0xff, 0x01, 0xfe, 0x01, 0xff, 0x00, 0x00, 0xff, 0xff, 0x1f,
    0xf8, 0xff, 0x07, 0x00, 0xc0, 0xff, 0x01, 0xe0, 0xff, 0x0f, 0xf0, 0x07,
    0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xfe, 0xff, 0x01, 0x00,
    0xf8, 0x3f, 0x00, 0xfc, 0xff, 0x01, 0xfe, 0x01, 0xff, 0x00, 0x00, 0xfc,
    0xff, 0x1f, 0xf8, 0xff, 0x0f, 0x00, 0xc0, 0xff, 0x01, 0xe0, 0xff, 0x0f,
    0xf0, 0x07, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xfe, 0xff,
    0x01, 0x00, 0xf8, 0x3f, 0x00, 0xfc, 0xff, 0x01, 0xfe, 0x00, 0xff, 0x01,
    0x00, 0xf8, 0xff, 0x1f, 0xf0, 0xff, 0x0f, 0x00, 0xe0, 0xff, 0x00, 0xe0,
    0xff, 0x0f, 0xf0, 0x07, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
    0xfe, 0xff, 0x03, 0x00, 0xfc, 0x1f, 0x00, 0xfc, 0xff, 0x01, 0xfe, 0x00,
    0xff, 0x01, 0x00, 0xf0, 0xff, 0x1f, 0xf0, 0xff, 0x0f, 0x00, 0xe0, 0xff,
    0x00, 0xe0, 0xff, 0x0f, 0xf0, 0x07, 0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff,
    0x1f, 0x00, 0xfc, 0xff, 0x03, 0x00, 0xfe, 0x1f, 0x00, 0xfc, 0xff, 0x01,
    0xfe, 0x00, 0xff, 0x03, 0x00, 0xf0, 0xff, 0x1f, 0xf0, 0xff, 0x1f, 0x00,
    0xf0, 0xff, 0x00, 0xe0, 0xff, 0x0f, 0xf0, 0x07, 0x80, 0xff, 0xff, 0x00,
    0xc0, 0xff, 0x1f, 0x00, 0xfc, 0xff, 0x07, 0x00, 0xff, 0x1f, 0x00, 0xfc,
    0xff, 0x01, 0xfe, 0x00, 0xff, 0x07, 0x00, 0xf0, 0xff, 0x0f, 0xe0, 0xff,
    0x3f, 0x00, 0xf8, 0x7f, 0x00, 0xe0, 0xff, 0x0f, 0xf8, 0x07, 0xc0, 0xff,
    0xff, 0x00, 0xe0, 0xff, 0x3f, 0x00, 0xf8, 0xff, 0x0f, 0x80, 0xff, 0x0f,
    0x00, 0xfc, 0xff, 0x03, 0xff, 0x00, 0xff, 0x1f, 0x00, 0xf0, 0xff, 0x0f,
    0xe0, 0xff, 0x7f, 0x00, 0xfc, 0x7f, 0x00, 0xe0, 0xff, 0x1f, 0xf8, 0x07,
    0xfe, 0xff, 0xff, 0x1f, 0xf0, 0xff, 0x7f, 0x00, 0xf8, 0xff, 0x1f, 0xe0,
    0xff, 0x07, 0x00, 0xfc, 0xff, 0x87, 0xff, 0x00, 0xff, 0x7f, 0x00, 0xf8,
    0xff, 0x07, 0xc0, 0xff, 0xff, 0x00, 0xff, 0x3f, 0x00, 0xe0, 0xff, 0x3f,
    0xfc, 0x07, 0xfe, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0x0f, 0xf0, 0xff,
    0xff, 0xff, 0xff, 0x07, 0x00, 0xf8, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
    0x03, 0xff, 0xff, 0x07, 0x80, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x00, 0xe0,
    0xff, 0xff, 0xff, 0x03, 0xfe, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0x0f,
    0xe0, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0xf8, 0xff, 0xff, 0x7f, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x80, 0xff, 0xff, 0xff, 0xff, 0x0f,
    0x00, 0xc0, 0xff, 0xff, 0xff, 0x03, 0xfe, 0xff, 0xff, 0x1f, 0xfe, 0xff,
    0xff, 0x0f, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xf0, 0xff, 0xff,
    0x7f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xff, 0xff, 0xff,
    0xff, 0x0f, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x01, 0xfe, 0xff, 0xff, 0x1f,
    0xfe, 0xff, 0xff, 0x0f, 0x80, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xf0,
    0xff, 0xff, 0x3f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xfc,
    0xff, 0xff, 0xff, 0x03, 0x00, 0x80, 0xff, 0xff, 0xff, 0x01, 0xfe, 0xff,
    0xff, 0x1f, 0xfe, 0xff, 0xff, 0x0f, 0x00, 0xff, 0xff, 0xff, 0x3f, 0x00,
    0x00, 0xe0, 0xff, 0xff, 0x1f, 0x00, 0xff, 0xfe, 0xff, 0xff, 0x7f, 0x00,
    0x00, 0xf8, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
    0xfe, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0x0f, 0x00, 0xfc, 0xff, 0xff,
    0x0f, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x0f, 0x00, 0xff, 0xf0, 0xff, 0xff,
    0x1f, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0xfe, 0xff,
    0x7f, 0x00, 0xfe, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0x0f, 0x00, 0xf0,
    0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0x07, 0x00, 0x7f, 0xc0,
    0xff, 0xff, 0x07, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00,
    0xfc, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xc0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x01, 0x00,
    0x00, 0x00, 0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x03, 0x00,
    0x00, 0x00, 0xf0, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00};

/********************
* START OF PROGRAM *
********************/

boolean colon;
boolean dirty;

uint8_t hour;
uint8_t minute;
uint8_t prevHour;
uint8_t prevMinute;

// Holds one of the MODE_XX constants to determine the mode
uint8_t currentMode;
// Updated via timer
uint8_t newMode;

Timer colonTimer(1000, toggleColon);
Timer timeoutTimer(BOOTSCREEN_TIMEOUT, timeout);

void setup() {
   // Publish functions to internet
   Particle.function("draw", inetDraw);
   Particle.function("clear", inetClear);

   Serial.begin(9600);

   // Clear the buffer.
   display.begin(SSD1306_SWITCHCAPVCC);
   display.clearDisplay();
   display.display();

   Time.zone(2);
   setMode(MODE_BOOT);
}

void timeout() {
   setMode(MODE_CLOCK);
}

void setMode(uint8_t mode) {
   newMode = mode;
   timeoutTimer.reset();
}

int getMode() {
   return currentMode;
}

void updateMode() {
   if (newMode == currentMode) {
      return;
   }
   colonTimer.stop();
   timeoutTimer.stop();
   currentMode = newMode;

   if (currentMode == MODE_BOOT) {
      display.clearDisplay();
      drawBootScreen();
      timeoutTimer.changePeriod(BOOTSCREEN_TIMEOUT);
      return;
   }
   if (newMode == MODE_CLOCK) {
      display.clearDisplay();
      updateTime();
      scrollNumbers(true);
      colonTimer.start();
      return;
   }
   if (newMode == MODE_SCROLL) {
      timeoutTimer.changePeriod(SCROLL_TIMEOUT);
      return;
   }
   if (newMode == MODE_DRAW) {
      timeoutTimer.changePeriod(DRAW_TIMEOUT);
      clearDown();
      return;
   }
}

int inetDraw(String data) {
   setMode(MODE_DRAW);
   while (data.length() > 0) {
      int x = data.substring(0, 3).toInt();
      int y = data.substring(3, 5).toInt();
      int value = data.substring(5, 6).toInt();
      display.drawPixel(x, y, value);
      data = data.substring(6);
   }
   display.display();
   return 1;
}

int inetClear(String notUsed) {
   // Already in mode draw, then clear directly.
   if (getMode() == MODE_DRAW) {
      clearDown();
   } else {
      setMode(MODE_DRAW);
   }
   return 0;
}

void loop() {
   updateMode();

   if (currentMode == MODE_CLOCK) {
      delay(50);
      updateTime();
      scrollNumbers(false);
      if (dirty) {
         display.display();
         dirty = false;
      }

      if (random(0, 10000) == 0) {
         setMode(MODE_SCROLL);
      }
      return;
   }

   if (currentMode == MODE_SCROLL) {
      scrollNetset();
      return;
   }

   if (currentMode == MODE_DRAW) {
      return;
   }

   if (currentMode == MODE_BOOT) {
      return;
   }
}

void toggleColon() {
   if (colon) {
      drawColon();
   } else {
      clearColon();
   }
   colon = !colon;
   dirty = true;
}

void scrollNumbers(boolean force) {
   if (minute % 10 != prevMinute % 10 || force) {
      scrollNumber(minute % 10, 100);
   }
   if (minute / 10 != prevMinute / 10 || force) {
      scrollNumber(minute / 10, 72);
   }
   if (hour % 10 != prevHour % 10 || force) {
      scrollNumber(hour % 10, 31);
   }
   if (hour / 10 != prevHour / 10 || force) {
      scrollNumber(hour / 10, 3);
   }
   prevHour = hour;
   prevMinute = minute;
}

void scrollNumber(uint16_t number, uint16_t xPos) {
   uint8_t a;
   for (a = 0; a < NUMBERS_HEIGHT; a++) {
      display.shiftDisplayDown(xPos, NUMBERS_WIDTH);
      drawNumberOnFirstRow(number, NUMBERS_HEIGHT - a - 1, xPos);
      display.display();
      delay(NUMBERS_HEIGHT - a);
   }
   for (a = 0; a < SSD1306_LCDHEIGHT - NUMBERS_HEIGHT + BOUNCE_AMOUNT; a++) {
      display.shiftDisplayDown(xPos, NUMBERS_WIDTH);
      display.display();
   }
   for (a = 0; a < BOUNCE_AMOUNT; a++) {
      display.shiftDisplayUp(xPos, NUMBERS_WIDTH);
      display.display();
   }
}

void scrollNetset() {
   uint16_t cnt = 0;
   uint8_t step = 1;

   while (step != 0) {
      if (step == 1) {
         clearDown();
         step = 2;
      }
      if (step == 2) {
         delay(1000);
         cnt = 0;
         step = 3;
      }
      if (step == 3) {
         if (cnt == BITMAP_WIDTH * 8) {
            cnt = 0;
            step = 4;
         } else {
            drawFrame(cnt);
            cnt++;
         }
      }
      if (step == 4) {
         if (cnt == SSD1306_LCDWIDTH) {
            cnt = 0;
            step = 5;
         } else {
            drawBlackFrame();
            cnt++;
         }
      }
      if (step == 5) {
         delay(2000);
         cnt = 0;
         step = 0;
      }
   }
}

void drawBootScreen() {
   display.drawXBitmap(36, 10, BOOTSCREEN_BITS, BOOTSCREEN_WIDTH, BOOTSCREEN_HEIGHT, 1);
   display.display();
}

void clearDown() {
   uint8_t a;
   for (a = 0; a < SSD1306_LCDHEIGHT; a++) {
      display.shiftDisplayDown(0, SSD1306_LCDWIDTH);
      display.display();
      delay(5);
   }
}

void drawFrame(uint16_t cnt) {
   display.shiftDisplayLeft();
   drawColumn(cnt);
   display.display();
}

void drawBlackFrame() {
   //    uint8_t y;
   display.shiftDisplayLeft();
   //    for (y = 0; y < SSD1306_LCDHEIGHT; y++) {
   //       display.drawPixel(SSD1306_LCDWIDTH - 1, y, 0);
   //    }
   display.display();
}

void drawColumn(int16_t x) {
   uint8_t y, data;
   for (y = 0; y < SSD1306_LCDHEIGHT; y++) {
      data = BITMAP_BITS[(x / 8) + y * BITMAP_WIDTH] & 1 << x % 8;
      display.drawPixel(SSD1306_LCDWIDTH - 1, y, data > 0 ? 1 : 0);
   }
}

void drawNumberOnFirstRow(int16_t number, int16_t bitmapY, int16_t screenX) {
   uint16_t x, data;
   uint16_t index = number * NUMBERS_WIDTH / 8 * NUMBERS_HEIGHT;  // Points to number in the bitmap
   index = index + bitmapY * NUMBERS_WIDTH / 8;                   // Points to correct line in the number

   for (x = 0; x < NUMBERS_WIDTH; x++) {
      data = NUMBERS_BITS[index + (x / 8)] & (1 << x % 8);
      display.drawPixel(screenX + x, 0, data > 0 ? 1 : 0);
   }
}

void updateTime() {
   hour = Time.hour();
   minute = Time.minute();
}

// void drawNumber(uint8_t xpos, uint8_t number) {
//    uint8_t* index = NUMBERS_BITS + (number * NUMBERS_WIDTH * NUMBERS_HEIGHT / 8);
//    display.drawXBitmap(xpos, 6, index, NUMBERS_WIDTH, NUMBERS_HEIGHT, 1);
// }

void drawColon() {
   display.drawXBitmap(60, 20, COLON_BITS, COLON_WIDTH, COLON_HEIGHT, 1);
}

void clearColon() {
   display.drawRect(60, 20, COLON_WIDTH, COLON_HEIGHT, 0);
}
