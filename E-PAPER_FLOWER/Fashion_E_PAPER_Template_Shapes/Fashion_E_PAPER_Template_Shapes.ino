#include <GxEPD2_3C.h>
#include <SPI.h>
#include "flower_bitmaps.h"
#include "logo_bitmaps.h"
#include "x_bitmaps.h"
#include "scratch_bitmaps.h"

#define EPD_CS   0
#define EPD_DC   1
#define EPD_RST  2
#define EPD_BUSY 21

#define EPD_SCK  19
#define EPD_MOSI 17

#define TOUCH_PIN 18  // Pin connected to the I/O of the touch sensor

// Image selection (Flower, X, Logo)
int currentImage = 0;  // 0: Flower, 1: X, 2: UG Logo; 3: Scratch Logo
int touchCount = 0;    // Counter for consecutive touches
bool isImageLoaded = false;  // Flag to check if image is fully loaded
unsigned long touchStartTime = 0;  // To track touch start time
unsigned long lastTouchTime = 0;  // To track last touch time
unsigned long inactivityTimer = 0;  // To track the inactivity time

GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(
  GxEPD2_154_Z90c(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting touch sensor and image rotation test...");

  SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);

  display.init(115200, true, 2, false);
  display.setRotation(0);
  display.setFullWindow();

  pinMode(TOUCH_PIN, INPUT);  // Set touch pin as input

  // Step 1: Clear the screen and set default index
  Serial.println("Clearing screen...");
  display.fillScreen(GxEPD_WHITE);  // Clear the screen
  currentImage = 0;  // Default to "Flower" image

  // Step 2: Draw the default image (Flower) on startup
  Serial.println("Displaying Startup Graphic...");
  display.firstPage();
  do {
    drawImage(currentImage);  // Draw the Flower image
  } while (display.nextPage());

  Serial.println("Flower displayed.");
  delay(2000);  // 2-second buffer after first image load

  isImageLoaded = true;  // Image is fully loaded, can check touch now
}

void drawImage(int imageIndex)
{
  int x_offset = 0;  // Declare variables outside of switch
  int y_offset = 0;

  display.fillScreen(GxEPD_WHITE);  // Clear the screen
  
  switch (imageIndex) {
    case 0:  // Flower
      Serial.println("Displaying Flower...");
      display.drawBitmap(0, 0, redBitmap, FLOWER_W, FLOWER_H, GxEPD_RED);
      display.drawBitmap(0, 0, blackBitmap, FLOWER_W, FLOWER_H, GxEPD_BLACK);
      break;
    case 1:  // X
      Serial.println("Displaying X...");
      display.drawBitmap(0, 0, xBitmap, X_W, X_H, GxEPD_BLACK);
      break;
    case 2:  // Upstairs Logo
      Serial.println("Displaying UG Logo...");
      // Calculate the center position for the logo
      x_offset = (200 - LOGO_W) / 2;
      y_offset = (200 - LOGO_H) / 2;
      display.drawBitmap(x_offset, y_offset, logoBitmap, LOGO_W, LOGO_H, GxEPD_BLACK);
      break;
    case 3: // Scratch Logo
    Serial.println("Displaying Scratch Logo...");
    display.drawBitmap(0, 0, scratchBitmap, SCRATCH_W, SCRATCH_H, GxEPD_BLACK);
    default:
      break;
  }
}

void loop()
{
  if (isImageLoaded) {
    // Only check for touch if image is fully loaded
    int touchState = digitalRead(TOUCH_PIN);
    unsigned long currentTime = millis();  // Get the current time

  // If no touch detected for 2 seconds, reset touch count
  if (touchState == LOW && (currentTime - lastTouchTime) > 2000) {
    touchCount = 0;
    Serial.println("No touch detected for 2 seconds. Resetting touch count.");
  }
    
    // If touch is detected, increment the touch count
    if (touchState == HIGH) {
      touchCount++;
      Serial.print("Touch detected! Count: ");
      Serial.println(touchCount);
      lastTouchTime = currentTime;  // Update last touch time
      inactivityTimer = currentTime; // Reset inactivity timer
      delay(300);
    }

    // If 5 consecutive touches detected, change image
    if (touchCount >= 5) {
      Serial.println("5 consecutive touches detected. Changing image...");
      currentImage = (currentImage + 1) % 4;  // Cycle through 0, 1, 2 (Flower, X, Logo)

      // Reset touch count after switching image
      touchCount = 0;
      isImageLoaded = false;  // Prevent further touch checks until image is rendered

      // Render the new image
      display.firstPage();
      do {
        drawImage(currentImage);
      } while (display.nextPage());

      // Wait for 2 seconds before allowing next touch detection
      delay(2000);  
      
      isImageLoaded = true;  // Image has been loaded, can check for touch again
    }
  }
}