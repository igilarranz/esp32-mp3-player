#include <Arduino.h>
#include "driver/i2s.h"

#define I2S_DOUT 16
#define I2S_BCLK 17
#define I2S_LRC  15

#define SAMPLE_RATE 44100
#define I2S_PORT I2S_NUM_0

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("=== Speaker test (lower volume) ===");

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
  i2s_zero_dma_buffer(I2S_PORT);
  Serial.println("I2S ready - tone playing");
}

void loop() {
  static float phase = 0;
  const float phaseInc = 2.0 * PI * 440.0 / SAMPLE_RATE;
  int16_t samples[256];
  size_t bytesWritten;

  for (int i = 0; i < 128; i++) {
    int16_t s = 8000 * sin(phase);   // lowered from 16000 to 8000
    samples[i * 2] = s;
    samples[i * 2 + 1] = s;
    phase += phaseInc;
    if (phase > 2 * PI) phase -= 2 * PI;
  }

  i2s_write(I2S_PORT, samples, sizeof(samples), &bytesWritten, portMAX_DELAY);
}