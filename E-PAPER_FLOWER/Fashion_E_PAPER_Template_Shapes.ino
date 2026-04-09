#include <GxEPD2_3C.h>
#include <SPI.h>

#define EPD_CS   0
#define EPD_DC   1
#define EPD_RST  2
#define EPD_BUSY 21

#define EPD_SCK  19
#define EPD_MOSI 17

GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(
  GxEPD2_154_Z90c(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

void drawTestShapes()
{
  display.fillScreen(GxEPD_WHITE);

  // Black circle outline
  display.drawCircle(50, 50, 25, GxEPD_BLACK);

  // Filled red circle
  display.fillCircle(150, 50, 20, GxEPD_RED);

  // Black square
  display.drawRect(20, 110, 40, 40, GxEPD_BLACK);

  // Filled black square
  display.fillRect(70, 110, 40, 40, GxEPD_BLACK);

  // Red square outline
  display.drawRect(120, 110, 40, 40, GxEPD_RED);

  // Filled red square
  display.fillRect(70, 160, 50, 30, GxEPD_RED);

  // Small mixed markers
  display.fillCircle(150, 150, 8, GxEPD_BLACK);
  display.fillCircle(175, 150, 8, GxEPD_RED);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting shape test...");

  SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);

  display.init(115200, true, 2, false);
  display.setRotation(0);
  display.setFullWindow();

  display.firstPage();
  do {
    drawTestShapes();
  } while (display.nextPage());

  Serial.println("Shape test done.");
}

void loop() {
}