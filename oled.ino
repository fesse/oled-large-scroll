#include <application.h>

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "oled_graphics.h"

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
 * Solar Power
 **/
#define MODE_BOOT 1
#define MODE_CLOCK 2
#define MODE_SCROLL 3
#define MODE_DRAW 4
#define MODE_SOLAR 5

#define CLOCK_HOUR_1_X 3
#define CLOCK_HOUR_2_X 31
#define CLOCK_MINUTE_1_X 72
#define CLOCK_MINUTE_2_X 100
#define CLOCK_Y 4
#define CLOCK_COLON_X 60
#define CLOCK_COLON_Y 20

#define BOUNCE_AMOUNT 15

#define DRAW_TIMEOUT 60000
#define BOOTSCREEN_TIMEOUT 3000
#define SCROLL_TIMEOUT 1000
#define SOLAR_TIMEOUT 10000

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

// Updated via solarPowerHandler;
uint16_t solarPower = 0;

Timer colonTimer(1000, toggleColon);
Timer timeoutTimer(BOOTSCREEN_TIMEOUT, timeout);

void setup() {
   // Publish functions to internet
   Particle.function("draw", inetDraw);
   Particle.function("clear", inetClear);

   // Subscribe to Solar panel effect report
   Particle.subscribe("hook-response/getSolarEffect", solarPowerHandler, MY_DEVICES);

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

void solarPowerHandler(const char *event, const char *data) {
   solarPower = atoi(data);
   if (solarPower > 0) {
      setMode(MODE_SOLAR);
   }
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
      updateTime();
      displayClock();
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
   if (newMode == MODE_SOLAR) {
      timeoutTimer.changePeriod(SOLAR_TIMEOUT);
      displaySolarEffect();
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
      updateClock();
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

   if (currentMode == MODE_SOLAR) {
      return;
   }

   if (currentMode == MODE_DRAW) {
      return;
   }

   if (currentMode == MODE_BOOT) {
      return;
   }
}

void displaySolarEffect() {
   uint8_t yPos = 0;
   int16_t wait;

   for (yPos = 1; yPos <= SSD1306_LCDHEIGHT; yPos++) {
      display.shiftDisplayDown(0, SSD1306_LCDWIDTH);
      if (yPos <= NUMBERS_HEIGHT) {
         if (solarPower >= 10000) {
            drawNumberOnFirstRow((solarPower / 10000) % 10, NUMBERS_HEIGHT - yPos, 0);
         }
         if (solarPower >= 1000) {
            drawNumberOnFirstRow((solarPower / 1000) % 10, NUMBERS_HEIGHT - yPos, 25);
         }
         if (solarPower >= 100) {
            drawNumberOnFirstRow((solarPower / 100) % 10, NUMBERS_HEIGHT - yPos, 51);
         }
         if (solarPower >= 10) {
            drawNumberOnFirstRow((solarPower / 10) % 10, NUMBERS_HEIGHT - yPos, 77);
         }
         if (solarPower > 0) {
            drawNumberOnFirstRow(solarPower % 10, NUMBERS_HEIGHT - yPos, 103);
         }
      }

      display.display();
      wait = SSD1306_LCDHEIGHT - (yPos * 1.5);
      wait = abs(wait);
      delay(wait);
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

void displayClock() {
   // Scroll up from bottom
   uint8_t yPos = 0;
   int16_t wait;

   for (yPos = 0; yPos < 10; yPos++) {
      display.shiftDisplayUp(0, SSD1306_LCDWIDTH);
      display.display();
      delay(SSD1306_LCDHEIGHT);
   }

   for (yPos = 0; yPos < NUMBERS_HEIGHT; yPos++) {
      display.shiftDisplayUp(0, SSD1306_LCDWIDTH);
      if (yPos <= NUMBERS_HEIGHT) {
         drawNumberOnLastRow((hour / 10) % 10, yPos, CLOCK_HOUR_1_X);
         drawNumberOnLastRow(hour % 10, yPos, CLOCK_HOUR_2_X);
         drawNumberOnLastRow((minute / 10) % 10, yPos, CLOCK_MINUTE_1_X);
         drawNumberOnLastRow(minute % 10, yPos, CLOCK_MINUTE_2_X);
      }
      display.display();
      wait = SSD1306_LCDHEIGHT - (yPos * 1.5);
      wait = abs(wait);
      delay(wait);
   }
   prevHour = hour;
   prevMinute = minute;
}

void updateClock() {
   if (minute % 10 != prevMinute % 10) {
      scrollClockNumber(minute % 10, CLOCK_MINUTE_2_X);
   }
   if (minute / 10 != prevMinute / 10) {
      scrollClockNumber(minute / 10, CLOCK_MINUTE_1_X);
   }
   if (hour % 10 != prevHour % 10) {
      scrollClockNumber(hour % 10, CLOCK_HOUR_2_X);
   }
   if (hour / 10 != prevHour / 10) {
      scrollClockNumber(hour / 10, CLOCK_HOUR_1_X);
   }
   prevHour = hour;
   prevMinute = minute;
}

void scrollClockNumber(uint16_t number, uint16_t xPos) {
   uint8_t yPos = 0;
   int8_t wait;

   // Move down
   for (yPos = 1; yPos <= SSD1306_LCDHEIGHT + BOUNCE_AMOUNT; yPos++) {
      display.shiftDisplayDown(xPos, NUMBERS_WIDTH);
      if (yPos <= NUMBERS_HEIGHT) {
         drawNumberOnFirstRow(number, NUMBERS_HEIGHT - yPos, xPos);
      }
      display.display();
      wait = SSD1306_LCDHEIGHT - yPos;
      delay(constrain(wait, 0, 60));
   }

   // Bounce up
   for (yPos = SSD1306_LCDHEIGHT + BOUNCE_AMOUNT; yPos > SSD1306_LCDHEIGHT; yPos--) {
      display.shiftDisplayUp(xPos, NUMBERS_WIDTH);
      if (yPos > SSD1306_LCDHEIGHT) {
         drawNumberOnLastRow(number, NUMBERS_HEIGHT - (yPos - SSD1306_LCDHEIGHT), xPos);
      }
      display.display();
      wait = 60 - ((yPos - SSD1306_LCDHEIGHT) * 10);
      wait = constrain(wait, 0, 60);
      delay(wait);
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
   display.shiftDisplayLeft();
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

void drawNumberOnLastRow(int16_t number, int16_t bitmapY, int16_t screenX) {
   uint16_t x, data;
   uint16_t index = number * NUMBERS_WIDTH / 8 * NUMBERS_HEIGHT;  // Points to number in the bitmap
   index = index + bitmapY * NUMBERS_WIDTH / 8;                   // Points to correct line in the number

   for (x = 0; x < NUMBERS_WIDTH; x++) {
      data = NUMBERS_BITS[index + (x / 8)] & (1 << x % 8);
      display.drawPixel(screenX + x, SSD1306_LCDHEIGHT - 1, data > 0 ? 1 : 0);
   }
}

void updateTime() {
   hour = Time.hour();
   minute = Time.minute();
}

void drawColon() {
   display.drawXBitmap(CLOCK_COLON_X, CLOCK_COLON_Y, COLON_BITS, COLON_WIDTH, COLON_HEIGHT, 1);
}

void clearColon() {
   display.drawRect(CLOCK_COLON_X, CLOCK_COLON_Y, COLON_WIDTH, COLON_HEIGHT, 0);
}
