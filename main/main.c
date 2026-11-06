#include <stdio.h>
#include <string.h>

#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Periphery.h"

#include "AddressStrip.h"
#include "Settings.h"
#include "BLEServer.h"

void SendData_WS2812B(WS2812B_st *ws2812b, WS2812B_PixelColorRGB_st *pixelColor, uint16_t count) {
  RMT_Transmit((void *)pixelColor, count * 3);
}

uint32_t GetTick(){
	return esp_timer_get_time() / 1000;
}

WS2812B_st strip = {};
WS2812B_UserFunctions_st stripFunctions = {.sendData = SendData_WS2812B, .getTick = GetTick};

AddressStrip_st addressStrip = {0};

uint32_t settingsChangeTime = 0;

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

void Settings_Task(){
	while (1) {
		if(SettingsGetUpdateFlag() && (settingsChangeTime + 5000 < GetTick())){
			SettingsSetUpdateFlag(0);
			Settings_Save();
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void app_main(void) {
  RMT_Init();
  I2S_Init();
  NVS_Init();

  Settings_Init();
  BLEServer_Init("LED");


  WS2812B_Init(&strip, &stripFunctions);
  AddressStrip_Init(&addressStrip, &strip);
  
  AddressStrip_SetNumberLeds(&addressStrip, Settings_GetNumLeds());
  
  AddressStrip_SetState(&addressStrip, Settings_GetState());
  
  AddressStrip_SetBrightness(&addressStrip, Settings_GetBrightness());
  
  AddressStrip_SetHue(&addressStrip, Settings_GetHue());
  AddressStrip_SetSaturation(&addressStrip, Settings_GetSaturation());
  
  AddressStrip_SetEffect(&addressStrip,Settings_GetEffect());
  AddressStrip_SetEffectSpeed(&addressStrip,Settings_GetEffectSpeed());
  
  AddressStrip_SetColorMusic(&addressStrip,Settings_GetColorMusic());
  AddressStrip_SetColorMusicSpeed(&addressStrip,Settings_GetColorMusicSpeed());
  
  AddressStrip_SetMode(&addressStrip, Settings_GetMode());
  
  xTaskCreate(AddressStrip_Task, NULL, 8192, NULL, 20, NULL);
  xTaskCreate(Microphone_Task, NULL, 4096, NULL, 0, NULL);
  xTaskCreate(Settings_Task, NULL, 4096, NULL, 0, NULL);
}

void BLEServer_ClientRead_Callback(uint8_t* data, uint16_t* len){
	data[0] = 0x12;
	data[1] = 0x34;
	*len = 2;
}

void BLEServer_ClientWrite_Callback(uint8_t* data, uint16_t* len){
	printf("BLEServer_ClientWrite_Callback\r\n");
	for(int i = 0; i < *len; i++){
		printf("data[%d] = %d\r\n", i, data[i]);
	}
	
	if(data[0] == 0x00){
		uint16_t numled = (data[1] << 8) | data[2];
		printf("numled = %d\n", numled);
		AddressStrip_SetNumberLeds(&addressStrip, numled);
		Settings_SetNumLeds(numled);
	}
	if(data[0] == 0x01){
		AddressStrip_SetState(&addressStrip, data[1]);
		Settings_SetState(data[1]);
	}	
	if(data[0] == 0x02){
		AddressStrip_SetMode(&addressStrip, data[1]);
		Settings_SetMode(data[1]);
	}
	if(data[0] == 0x03){
		AddressStrip_SetBrightness(&addressStrip, data[1]);
		Settings_SetBrightness(data[1]);
	}
	if(data[0] == 0x04){
		uint16_t hue = (data[1] << 8) | data[2];
		AddressStrip_SetHue(&addressStrip, hue);
		Settings_SetHue(hue);
	}
	if(data[0] == 0x05){
		AddressStrip_SetSaturation(&addressStrip, data[1]);
		Settings_SetSaturation(data[1]);
	}
	if(data[0] == 0x06){
		AddressStrip_SetEffect(&addressStrip, data[1]);
		Settings_SetEffect(data[1]);
	}
	if(data[0] == 0x07){
		AddressStrip_SetEffectSpeed(&addressStrip, data[1]);
		Settings_SetEffectSpeed(data[1]);
	}
	if(data[0] == 0x08){
		AddressStrip_SetColorMusic(&addressStrip, data[1]);
		Settings_SetColorMusic(data[1]);
	}
	if(data[0] == 0x09){
		AddressStrip_SetColorMusicSpeed(&addressStrip, data[1]);
		Settings_SetColorMusicSpeed(data[1]);
	}					
	SettingsSetUpdateFlag(1);
	settingsChangeTime = GetTick();	
}
