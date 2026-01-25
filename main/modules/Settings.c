#include "Settings.h"

Settings_st settings = {0};

void Settings_Init(){
	SPIFFS_Init("/storage");
	uint8_t rb = SPIFFS_ReadFile("/storage/settings.bin", (uint8_t*)&settings, sizeof(Settings_st));
	if(rb != sizeof(Settings_st)){
		Settings_SetDefault();
		Settings_Save();
	}
}

void Settings_SetDefault(){
	settings.id = 0;
	settings.state = 1;
	settings.numLeds = 60;
}

void Settings_Save(){
	SPIFFS_WriteFile("/storage/settings.bin", (uint8_t*)&settings, sizeof(Settings_st));
}

void Settings_SetHue(uint8_t hue){
	settings.hue = hue;
}

uint8_t Settings_GetHue(){
	return settings.hue;
}