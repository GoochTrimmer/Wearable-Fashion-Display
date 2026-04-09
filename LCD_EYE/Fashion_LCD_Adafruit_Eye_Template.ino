#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_CS   2
#define TFT_DC   1
#define TFT_RST  0
#define TFT_BL   21

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const int SCREEN_W = 240;
const int SCREEN_H = 240;

// Eye geometry
const int EYE_X = 40;
const int EYE_Y = 70;
const int EYE_W = 160;
const int EYE_H = 100;
const int EYE_R = 50;

const int EYE_CX = 120;
const int EYE_CY = 120;

const int IRIS_R = 26;
const int PUPIL_R = 11;
const int HILITE_R = 4;

// Motion
float pupilX = 0;
float pupilY = 0;
float targetX = 0;
float targetY = 0;

int prevDrawX = EYE_CX;
int prevDrawY = EYE_CY;

unsigned long lastTargetTime = 0;

// Draw static eye once
void drawStaticEye() {
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(EYE_X, EYE_Y, EYE_W, EYE_H, EYE_R, ST77XX_WHITE);
}

// Restore only the old iris zone back to white
void eraseOldIris(int oldX, int oldY) {
  tft.fillCircle(oldX, oldY, IRIS_R + 2, ST77XX_WHITE);

  // Clean edges that may extend slightly from pupil/highlight redraw
  tft.fillCircle(oldX, oldY, PUPIL_R + 2, ST77XX_WHITE);
  tft.fillCircle(oldX - 8, oldY - 8, HILITE_R + 2, ST77XX_WHITE);
}

// Draw iris/pupil/highlight
void drawIris(int x, int y) {
  tft.fillCircle(x, y, IRIS_R, ST77XX_BLUE);
  tft.fillCircle(x, y, PUPIL_R, ST77XX_BLACK);
  tft.fillCircle(x - 8, y - 8, HILITE_R, ST77XX_WHITE);
}

void chooseNewTarget() {
  targetX = random(-18, 19);
  targetY = random(-12, 13);
}

void setup() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  SPI.begin(19, -1, 18, TFT_CS);
  tft.init(240, 240);
  tft.setRotation(0);

  randomSeed(millis());

  drawStaticEye();
  drawIris(EYE_CX, EYE_CY);
  chooseNewTarget();
}

void loop() {
  unsigned long now = millis();

  if (now - lastTargetTime > 1200) {
    chooseNewTarget();
    lastTargetTime = now;
  }

  pupilX += (targetX - pupilX) * 0.08f;
  pupilY += (targetY - pupilY) * 0.08f;

  int newX = EYE_CX + (int)pupilX;
  int newY = EYE_CY + (int)pupilY;

  // Only redraw if position actually changed
  if (newX != prevDrawX || newY != prevDrawY) {
    eraseOldIris(prevDrawX, prevDrawY);
    drawIris(newX, newY);
    prevDrawX = newX;
    prevDrawY = newY;
  }

  delay(30);
}