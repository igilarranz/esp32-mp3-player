#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  14
#define TFT_MOSI 11
#define TFT_MISO 13
#define TFT_SCK  12

// Use software SPI to bypass S3 hardware SPI weirdness
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Display test starting...");

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.setCursor(20, 40);
  tft.println("Music");
  tft.setCursor(20, 80);
  tft.println("Player");

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(20, 160);
  tft.println("Display OK!");

  Serial.println("Display init done");
}

void loop() {
  delay(1000);
}