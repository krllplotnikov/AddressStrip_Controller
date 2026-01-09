#ifndef SPIFFS_H_
#define SPIFFS_H_

#include <stdio.h>
#include "esp_spiffs.h"

void SPIFFS_Init(char* path);
size_t SPIFFS_ReadFile(char* path, uint8_t* data, size_t size);
size_t SPIFFS_WriteFile(char* path, uint8_t* data, size_t size);

#endif