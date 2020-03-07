#include "Speaker.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "i2s.h"
#include "sysctl.h"
#include "fpioa.h"
#include "uarths.h"
#include "gpio.h"
#include "gpiohs.h"

SPEAKER::SPEAKER(void) {
  _volume = 8;
  _begun = false;
}

void SPEAKER::begin() {
  _begun = true;
//  ledcSetup(TONE_PIN_CHANNEL, 0, 13);
//  ledcAttachPin(SPEAKER_PIN, TONE_PIN_CHANNEL);
//  sysctl_pll_set_freq(SYSCTL_PLL0, 320000000UL);
//  sysctl_pll_set_freq(SYSCTL_PLL1, 160000000UL);
//  sysctl_pll_set_freq(SYSCTL_PLL2, 45158400UL);
  pll_init();
  uarths_init();
  // io_mux_init
  fpioa_set_function(I2S_DA, FUNC_I2S0_OUT_D1);
  fpioa_set_function(I2S_WS, FUNC_I2S0_WS);	
  fpioa_set_function(I2S_BCK, FUNC_I2S0_SCLK);
  i2s_init(I2S_DEVICE_0, I2S_TRANSMITTER, 0x0C);
  // Power Amp enable for NewMaixGo = pin 32 / Maixduino = pin 2
  fpioa_set_function(32, FUNC_GPIO1);
  gpio_set_drive_mode(1, GPIO_DM_OUTPUT);
  gpio_set_pin(1, GPIO_PV_HIGH);

  setBeep(1000, 1000);
}

void SPEAKER::end() {
  mute();
//  ledcDetachPin(SPEAKER_PIN);
  _begun = false;
}

void SPEAKER::generateSign(uint16_t frequency) {
  #define SAMPLE_RATE 16000
  //#define PI 3.14159265358979323846
//  int16_t FFT_N = SAMPLE_RATE / frequency;
  int16_t FFT_N = 2048;
  wavdata = new short[FFT_N];
  int i, temp;
	for (i = 0; i < FFT_N; i++) {
		temp = (short)(32767 * sinf(2 * PI * i / FFT_N) * (11 - _volume) / 11);
		wavdata[i] = (short)(temp);
	}
}

void SPEAKER::tone(uint16_t frequency) {
  if(!_begun) begin();
//  ledcWriteTone(TONE_PIN_CHANNEL, frequency);
//  generateSign(frequency);
  #define SAMPLE_RATE 16000
  //#define PI 3.14159265358979323846
  int16_t FFT_N = SAMPLE_RATE / frequency;
  short data[FFT_N];
  int i, temp;
	for (i = 0; i < FFT_N; i++) {
		temp = (short)(32767 * sinf(2 * PI * i / FFT_N) * (11 - _volume) / 11);
		data[i] = (short)(temp);
	}

  uint32_t act = i2s_set_sample_rate(I2S_DEVICE_0, SAMPLE_RATE);
  Serial.printf("Actual sampling rate: %u\r\n", act);

  i2s_tx_channel_config(I2S_DEVICE_0, I2S_CHANNEL_1,
      RESOLUTION_16_BIT, SCLK_CYCLES_32,
      /*TRIGGER_LEVEL_1*/TRIGGER_LEVEL_4,
      RIGHT_JUSTIFYING_MODE
      );

  while (millis() < _count) {
    i2s_play(I2S_DEVICE_0,
            DMAC_CHANNEL0, (uint8_t *)data, sizeof(data), 512, 16, 2);
	}
}

void SPEAKER::tone(uint16_t frequency, uint16_t duration) {
  _count = millis() + duration;
  speaker_on = 1;
  gpio_set_pin(1, GPIO_PV_HIGH);
  tone(frequency);
}

void SPEAKER::beep() {
  if(!_begun) begin();
  tone(_beep_freq, _beep_duration);
}

void SPEAKER::setBeep(uint16_t frequency, uint16_t duration) {
  _beep_freq = frequency;
  _beep_duration = duration;
}

void SPEAKER::setVolume(uint8_t volume) {
  _volume = 11 - volume;
}

void SPEAKER::mute() {
//  ledcWriteTone(TONE_PIN_CHANNEL, 0);
//  digitalWrite(SPEAKER_PIN, 0);
  gpio_set_pin(1, GPIO_PV_LOW);
}

void SPEAKER::update() {
  if(speaker_on) {
    if(millis() > _count) {
      speaker_on = 0;
      mute();
    }
  }
}

void SPEAKER::write(uint8_t value) {
//  dacWrite(SPEAKER_PIN, value);
}

void SPEAKER::playMusic(const uint8_t* music_data, uint16_t sample_rate) {
/*
  uint32_t length = strlen((char*)music_data);
  uint16_t delay_interval = ((uint32_t)1000000 / sample_rate);
  if(_volume != 11) {
    for(int i = 0; i < length; i++) {
      dacWrite(SPEAKER_PIN, music_data[i] / _volume);
      delayMicroseconds(delay_interval);
    }

    for(int t = music_data[length - 1] / _volume; t >= 0; t--) {
      dacWrite(SPEAKER_PIN, t);
      delay(2);
    }
  }
  // ledcSetup(TONE_PIN_CHANNEL, 0, 13);
  ledcAttachPin(SPEAKER_PIN, TONE_PIN_CHANNEL);
*/
  uint32_t length = strlen((char*)music_data);
  short data[length];
  if(_volume != 11) {
    for(int i = 0; i < length; i++) {
      data[i] = music_data[i] * 256 * (11 - _volume) / 11;
    }

    uint32_t act = i2s_set_sample_rate(I2S_DEVICE_0, SAMPLE_RATE);
    Serial.printf("Actual sampling rate: %u\r\n", act);

    i2s_tx_channel_config(I2S_DEVICE_0, I2S_CHANNEL_1,
      RESOLUTION_16_BIT, SCLK_CYCLES_32,
      /*TRIGGER_LEVEL_1*/TRIGGER_LEVEL_4,
      RIGHT_JUSTIFYING_MODE
      );

    while (millis() < _count) {
      i2s_play(I2S_DEVICE_0,
            DMAC_CHANNEL0, (uint8_t *)data, sizeof(data), 512, 16, 2);
  	}
  }
}
