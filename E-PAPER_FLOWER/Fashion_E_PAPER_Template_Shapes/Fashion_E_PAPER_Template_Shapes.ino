#include <GxEPD2_3C.h>
#include <SPI.h>
#include "flower_bitmaps.h"
#include "logo_bitmaps.h"
#include "x_bitmaps.h"

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
  Serial.println("Starting logo and flower test...");

  SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);

  display.init(115200, true, 2, false);
  display.setRotation(0);
  display.setFullWindow();

  Serial.println("Displaying X...");
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);  // Clear the screen
    
    display.drawBitmap(0, 0, xBitmap, X_W, X_H, GxEPD_BLACK);  // Draw the logo
  } while (display.nextPage());
  Serial.println("X displayed. Waiting for 5 seconds...");
  delay(5000);  // Wait for 5 seconds

  // Step 1: Show the logo bitmap
  Serial.println("Displaying logo...");
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);  // Clear the screen

    // Calculate the center position (X and Y)
    int x_offset = (200 - LOGO_W) / 2; // X offset to center horizontally
    int y_offset = (200 - LOGO_H) / 2; // Y offset to center vertically
    
    display.drawBitmap(x_offset, y_offset, logoBitmap, LOGO_W, LOGO_H, GxEPD_BLACK);  // Draw the logo
  } while (display.nextPage());
  Serial.println("Logo displayed. Waiting for 5 seconds...");
  delay(5000);  // Wait for 5 seconds

  // Step 2: show the flower bitmap
  Serial.println("Displaying flower...");
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);

    display.drawBitmap(0, 0, redBitmap, FLOWER_W, FLOWER_H, GxEPD_RED);
    display.drawBitmap(0, 0, blackBitmap, FLOWER_W, FLOWER_H, GxEPD_BLACK);

  } while (display.nextPage());

  Serial.println("Flower render complete.");
}

void loop() {}