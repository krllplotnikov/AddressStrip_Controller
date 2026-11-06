#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "SPIFFS.h"

typedef struct{
	uint8_t id;
	uint8_t state;
	uint8_t mode;
	
	uint16_t numLeds;
	uint8_t brightness;
	
	uint16_t hue;
	uint8_t saturation;
	
	uint8_t effect;
	uint8_t effectSpeed;
	
	uint8_t colorMusic;
	uint8_t colorMusicSpeed;
}Settings_st;

typedef struct{
	uint8_t updateFlag;	
}SettingsControl_st;

void Settings_Init();
void Settings_SetDefault();
void Settings_Save();

void Settings_SetNumLeds(uint16_t numLeds);
void Settings_SetState(uint8_t state);
void Settings_SetMode(uint8_t mode);
void Settings_SetBrightness(uint8_t brightness);
void Settings_SetHue(uint16_t hue);
void Settings_SetSaturation(uint8_t saturation);
void Settings_SetEffect(uint8_t effect);
void Settings_SetEffectSpeed(uint8_t effectSpeed);
void Settings_SetColorMusic(uint8_t colorMusic);
void Settings_SetColorMusicSpeed(uint8_t colorMusicSpeed);
void SettingsSetUpdateFlag(uint8_t flag);

uint16_t Settings_GetNumLeds();
uint8_t Settings_GetState();
uint8_t Settings_GetMode();
uint8_t Settings_GetBrightness();
uint16_t Settings_GetHue();
uint8_t Settings_GetSaturation();
uint8_t Settings_GetEffect();
uint8_t Settings_GetEffectSpeed();
uint8_t Settings_GetColorMusic();
uint8_t Settings_GetColorMusicSpeed();
uint8_t SettingsGetUpdateFlag();

#endif /* SETTINGS_H_ */