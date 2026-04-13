#include <GxEPD2_3C.h>
#include <SPI.h>
#include "flower_bitmaps.h"

#define EPD_CS   0
#define EPD_DC   1
#define EPD_RST  2
#define EPD_BUSY 21

#define EPD_SCK  19
#define EPD_MOSI 17

GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(
  GxEPD2_154_Z90c(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

void setup()
{
  Serial.begin(115200);
  delay(1000);

  SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);

  display.init(115200, true, 2, false);
  display.setRotation(0);
  display.setFullWindow();

  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);

    display.drawBitmap(0, 0, redBitmap, FLOWER_W, FLOWER_H, GxEPD_RED);
    display.drawBitmap(0, 0, blackBitmap, FLOWER_W, FLOWER_H, GxEPD_BLACK);

  } while (display.nextPage());

  Serial.println("Flower render complete.");
}

void loop() {}