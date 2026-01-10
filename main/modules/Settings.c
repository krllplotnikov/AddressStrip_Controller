#include "Settings.h"

Settings_st settings = {0};

void Settings_Init(){
	uint8_t rb = SPIFFS_ReadFile("/storage/settings.bin", (uint8_t*)&settings, sizeof(settings));
	if(rb != sizeof(settings)){
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
	SPIFFS_WriteFile("/storage/settings.bin", (uint8_t*)&settings, sizeof(settings));
}