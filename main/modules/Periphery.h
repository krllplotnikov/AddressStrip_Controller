#ifndef PERIPH_INIT_H_
#define PERIPH_INIT_H_

#include "driver/rmt_tx.h"
#include "driver/i2s.h"

#include "nvs_flash.h"
#include "esp_wifi.h"

#define NUM_SAMPLES 256
#define SAMPLE_RATE 16000

void RMT_Init();
void I2S_Init();

void NVS_Init();
void WiFi_Init();


void RMT_Transmit(void* payload, size_t payloadBytes);
int16_t* I2S_Read(size_t* bytesRead);

#endif
