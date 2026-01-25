#include <stdio.h>
#include <string.h>

#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Periphery.h"

#include "AddressStrip.h"
#include "Settings.h"

void SendData_WS2812B(WS2812B_st *ws2812b, WS2812B_PixelColor_st *pixelColor, uint16_t count) {
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
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void Change_Task(){
	while (1) {
		AddressStrip_SetEffectSpeed(&addressStrip, 0);
		vTaskDelay(pdMS_TO_TICKS(5000));
		AddressStrip_SetEffectSpeed(&addressStrip, 255);
		vTaskDelay(pdMS_TO_TICKS(5000));
		//AddressStrip_SetNumberLeds(&addressStrip, 360);
		//vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void app_main(void) {
  RMT_Init();
  I2S_Init();
  NVS_Init();

  Settings_Init();


  WS2812B_Init(&strip, &stripFunctions);
  AddressStrip_Init(&addressStrip, &strip);	
  
  AddressStrip_SetNumberLeds(&addressStrip, 10);
  AddressStrip_SetEffect(&addressStrip,ADDRESS_STRIP_EFFECT_RAINBOW);
  AddressStrip_SetMode(&addressStrip, ADDRESS_STRIP_MODE_EFFECT);
  
  xTaskCreate(AddressStrip_Task, NULL, 8192, NULL, 0, NULL);
  xTaskCreate(Change_Task, NULL, 8192, NULL, 0, NULL);
}
