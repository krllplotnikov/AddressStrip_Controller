#include <stdio.h>
#include <string.h>

#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Periphery.h"

#include "AddressStrip.h"
#include "Settings.h"

void SendData_WS2812B(WS2812B_st *ws2812b, WS2812B_PixelColorRGB_st *pixelColor, uint16_t count) {
  RMT_Transmit((void *)pixelColor, count);
}

uint32_t GetTick(){
	return esp_timer_get_time() / 1000;
}

WS2812B_st strip = {};
WS2812B_UserFunctions_st stripFunctions = {.sendData = SendData_WS2812B, .getTick = GetTick};

AddressStrip_st addressStrip = {0};

void AddressStrip_Task(){
	while (1) {
		AddressStrip_Run(&addressStrip);
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}

void Microphone_Task(){
	while (1) {
		if(AddressStrip_IsColorMusicReady(&addressStrip)){
			int16_t *pBuf = NULL;
    		size_t bytes = 0;
    		pBuf = I2S_Read(&bytes);
    		AddressStrip_UpdateColorMusicSignal(&addressStrip, pBuf);
		}
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}

void app_main(void) {
  RMT_Init();
  I2S_Init();
  NVS_Init();

  Settings_Init();


  WS2812B_Init(&strip, &stripFunctions);
  AddressStrip_Init(&addressStrip, &strip);
  
  AddressStrip_SetBrightness(&addressStrip, 10);
  AddressStrip_SetNumberLeds(&addressStrip, 60);
  
  AddressStrip_SetHue(&addressStrip, 60);
  AddressStrip_SetSaturation(&addressStrip, 100);
  
  AddressStrip_SetEffect(&addressStrip,ADDRESS_STRIP_EFFECT_RAINBOW_LOOP);
  AddressStrip_SetEffectSpeed(&addressStrip,25);
  
  AddressStrip_SetColorMusic(&addressStrip,ADDRESS_STRIP_COLOR_MUSIC_SPECTRE);
  AddressStrip_SetColorMusicSpeed(&addressStrip,25);
  
  AddressStrip_SetMode(&addressStrip, ADDRESS_STRIP_MODE_COLOR_MUSIC);
  
  xTaskCreate(AddressStrip_Task, NULL, 8192, NULL, 20, NULL);
  xTaskCreate(Microphone_Task, NULL, 4096, NULL, 0, NULL);
}
