#include "SPIFFS.h"

typedef struct{
	uint8_t id;
	uint8_t state;
	uint8_t mode;
	
	uint16_t numLeds;
	uint8_t brightness;
	
	uint8_t hue;
	uint8_t saturation;
	
	uint8_t effect;
	uint8_t effectSpeed;
	
	uint8_t colorMusic;
	uint8_t colorMusicSpeed;
}Settings_st;

void Settings_Init();
void Settings_SetDefault();
void Settings_Save();