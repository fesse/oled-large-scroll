#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include <application.h>

#define OLED_MOSI  A5 // HW SPI
#define OLED_CLK   A3 // HW SPI
#define OLED_DC    D3
#define OLED_CS    D4
#define OLED_RESET D5
// Hardware SPI
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

#define BITMAP_WIDTH 38 // Nbr chars
static const uint8_t bitmap_bits[] = {
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
   0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00 };

uint16_t cnt;
uint8_t step;

void setup() {
	Serial.begin(9600);

	display.begin(SSD1306_SWITCHCAPVCC);
	
	// Clear the buffer.
	display.clearDisplay();
	display.display();

	step = 1;
	cnt = 0;
}

void loop() {
	if (step == 1) {
		if (cnt == BITMAP_WIDTH*8) {
			cnt = 0;
			step = 2;
			return;
		}
		drawFrame(cnt);
		cnt++;
	}
	if (step == 2) {
		if (cnt == SSD1306_LCDWIDTH) {
			cnt = 0;
			step = 3;
			return;
		}
		drawBlackFrame();
		cnt++;
	}
	if (step == 3) {
		delay(100);
		cnt = 0;
		step = 1;
	}
}

void drawFrame(uint16_t cnt) {
	display.shiftDisplayLeft();
	drawColumn(cnt);
	display.display();
}

void drawBlackFrame() {
	display.shiftDisplayLeft();
	display.display();
}

void drawColumn(int16_t x) {
	uint8_t y, data;
	
	for (y=0; y<SSD1306_LCDHEIGHT; y++){
		data = bitmap_bits[(x/8) + y * BITMAP_WIDTH] & 1<<x%8;
		display.drawPixel(SSD1306_LCDWIDTH-1, y, data>0?1:0);
	}
}