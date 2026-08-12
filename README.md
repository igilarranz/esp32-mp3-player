# ESP32-S3 Portable MP3 Player

A handheld MP3 player built from scratch on an ESP32-S3: MP3 decoding from a microSD card, streamed over I2S to a class-D amplifier, with a color TFT display for the UI.

> **Status: work in progress.** SD-card MP3 playback and display bring-up are both working independently; UI integration is the current milestone. See [Roadmap](#roadmap).

![Display bring-up on the ESP32-S3](docs/hardware-photo.jpg)

## Hardware

| Component | Part | Role |
|---|---|---|
| MCU | ESP32-S3 Dev Module | MP3 decoding, SD access, display driving |
| Display | HiLetgo 2.2" ILI9341 SPI TFT, 240×320 | UI (module includes the microSD slot) |
| Amplifier | MAX98357A | I2S DAC + 3 W class-D amp |
| Storage | microSD card (slot on display module) | MP3 files |
| Power | USB (LiPo battery planned) | |

## Pin assignments

The TFT and microSD share one SPI bus (the SD slot is built into the display module); each has its own chip select.

| Signal | GPIO |
|---|---|
| SPI MOSI (shared) | 11 |
| SPI SCK (shared) | 12 |
| SPI MISO (shared) | 13 |
| TFT CS | 10 |
| TFT DC | 9 |
| TFT RST | 14 |
| SD CS | 18 |
| I2S BCLK | 17 |
| I2S LRC / WS | 15 |
| I2S DOUT | 16 |

## Repository layout

```
src/mp3_player/     Main sketch: SD → MP3 decode → I2S playback
tests/              Subsystem bring-up sketches
  i2s_tone_test/    Raw I2S driver test — generates a 440 Hz sine wave
  display_test/     ILI9341 init and text rendering test
docs/               Photos and wiring diagrams
```

## How it works

The main sketch uses the [ESP32-audioI2S](https://github.com/schreibfaul1/ESP32-audioI2S) library, which handles MP3 decoding on-chip and DMA-buffered I2S output. The sketch initializes the shared SPI bus, mounts the SD card, configures the I2S pinout for the MAX98357A, and streams a file from the card. Playback runs from `audio.loop()`; the library invokes callbacks (`audio_info`, `audio_eof_mp3`) for status and end-of-file events.

The I2S tone test in `tests/` goes a level lower: it configures the ESP-IDF I2S driver directly (DMA buffer sizing, pin mapping, 16-bit stereo frames) and synthesizes a sine wave sample-by-sample — useful for verifying the amp and speaker wiring independently of SD/decoding.

## Bring-up notes

- **ESP32-S3 hardware SPI + ILI9341:** the display initially failed with the hardware SPI constructor on the S3. Falling back to software SPI (explicit MOSI/SCK/MISO pins in the constructor) got the panel running; migrating back to hardware SPI is on the roadmap since software SPI is too slow for fluid UI redraws.
- **Shared SPI bus:** TFT and SD coexist on one bus with separate CS lines. SD is clocked at 4 MHz for reliable init.
- **Amp volume:** full-scale sine output was uncomfortably loud on the bare speaker; test tone amplitude is set to ~25% of full scale.

## Building and flashing

1. Arduino IDE with the ESP32 board package installed, board set to **ESP32S3 Dev Module**
2. Install libraries: **ESP32-audioI2S**, **Adafruit GFX**, **Adafruit ILI9341**
3. Put an MP3 on the SD card and update the file path in `mp3_player.ino` (default: `/music/track.mp3`)
4. Open `src/mp3_player/mp3_player.ino`, select the board's serial port, and upload
5. Open Serial Monitor at 115200 baud for status output

MP3 files are not included in this repo — bring your own.

## Roadmap

- [x] I2S audio output verified (sine wave test)
- [x] ILI9341 display bring-up
- [x] MP3 playback from microSD
- [ ] Integrate display + playback into a single firmware
- [ ] Track browser / playlist UI on the TFT
- [ ] Physical playback controls (play/pause, skip, volume)
- [ ] Move display back to hardware SPI for faster redraws
- [ ] LiPo battery + charging circuit for true portability
