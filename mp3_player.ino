/*
 * ESP32-S3 Portable MP3 Player — playback core
 *
 * Reads an MP3 file from a microSD card and streams it to a
 * MAX98357A I2S amplifier using the ESP32-audioI2S library.
 *
 * Hardware:
 *   - ESP32-S3 Dev Module
 *   - HiLetgo 2.2" ILI9341 TFT (240x320) with built-in microSD slot
 *   - MAX98357A I2S class-D amplifier
 *
 * The SD card slot on the display module shares the SPI bus with the TFT;
 * each device has its own chip-select line.
 *
 * Library: ESP32-audioI2S (schreibfaul1) — install via Library Manager
 * as "ESP32-audioI2S" or from https://github.com/schreibfaul1/ESP32-audioI2S
 */

#include <Arduino.h>
#include <Audio.h>
#include <SPI.h>
#include <SD.h>

// ---- SD card (shared SPI bus with TFT) ----
#define SD_CS    18
#define SD_MOSI  11
#define SD_SCK   12
#define SD_MISO  13

// ---- I2S audio (MAX98357A) ----
#define I2S_BCLK 17
#define I2S_LRC  15
#define I2S_DOUT 16

Audio audio;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("=== Music Player ===");

  // Init SD card
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, SPI, 4000000)) {
    Serial.println("SD init FAILED!");
    return;
  }
  Serial.println("SD initialized");

  // Init audio output
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(15);  // 0-21

  // Play a song from the SD card — change the path to match your file
  Serial.println("Starting playback...");
  audio.connecttoFS(SD, "/music/track.mp3");
}

void loop() {
  audio.loop();
}

// Optional callbacks for debug info
void audio_info(const char *info) {
  Serial.print("audio_info: ");
  Serial.println(info);
}

void audio_eof_mp3(const char *info) {
  Serial.print("Finished: ");
  Serial.println(info);
}
