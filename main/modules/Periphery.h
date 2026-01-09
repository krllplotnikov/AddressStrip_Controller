#ifndef PERIPH_INIT_H_
#define PERIPH_INIT_H_

#include "driver/rmt_tx.h"
#include "driver/i2s.h"

#include "nvs_flash.h"
#include "esp_wifi.h"

void RMT_Init();
void I2S_Init();

void NVS_Init();
void WiFi_Init();

#endif
