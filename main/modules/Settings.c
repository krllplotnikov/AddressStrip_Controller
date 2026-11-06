#include "Settings.h"
#include "AddressStrip.h"

Settings_st settings = {0};
SettingsControl_st settingsControl = {0};

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
	
	settings.numLeds = ADDRESS_STRIP_DEFAULT_NUMDER_LEDS;
	settings.state = ADDRESS_STRIP_DEFAULT_STATE;
	settings.mode = ADDRESS_STRIP_DEFAULT_MODE;
	settings.brightness = ADDRESS_STRIP_DEFAULT_BRIGHTNESS;
	settings.hue = ADDRESS_STRIP_DEFAULT_HUE;
	settings.saturation = ADDRESS_STRIP_DEFAULT_SATURATION;
	settings.effect = ADDRESS_STRIP_DEFAULT_EFFECT;
	settings.effectSpeed = ADDRESS_STRIP_DEFAULT_EFFECT_SPEED;
	settings.colorMusic = ADDRESS_STRIP_DEFAULT_COLOR_MUSIC;
	settings.colorMusicSpeed = ADDRESS_STRIP_DEFAULT_COLOR_MUSIC_SPEED;
}

void Settings_Save(){
	SPIFFS_WriteFile("/storage/settings.bin", (uint8_t*)&settings, sizeof(Settings_st));
}

void Settings_SetNumLeds(uint16_t numLeds){
	settings.numLeds = numLeds;
}

void Settings_SetState(uint8_t state){
	settings.state = state;
}

void Settings_SetMode(uint8_t mode){
	settings.mode = mode;
}

void Settings_SetBrightness(uint8_t brightness){
	settings.brightness = brightness;
}

void Settings_SetHue(uint16_t hue){
	settings.hue = hue;
}
void Settings_SetSaturation(uint8_t saturation){
	settings.saturation = saturation;
}

void Settings_SetEffect(uint8_t effect){
	settings.effect = effect;
}

void Settings_SetEffectSpeed(uint8_t effectSpeed){
	settings.effectSpeed = effectSpeed;
}

void Settings_SetColorMusic(uint8_t colorMusic){
	settings.colorMusic = colorMusic;
}

void Settings_SetColorMusicSpeed(uint8_t colorMusicSpeed){
	settings.colorMusicSpeed = colorMusicSpeed;
}

void SettingsSetUpdateFlag(uint8_t flag){
	settingsControl.updateFlag = flag;
}



uint16_t Settings_GetNumLeds(){
	return settings.numLeds;
}

uint8_t Settings_GetState(){
	return settings.state;
}

uint8_t Settings_GetMode(){
	return settings.mode;
}

uint8_t Settings_GetBrightness(){
	return settings.brightness;
}

uint8_t Settings_GetHue(){
	return settings.hue;
}
uint8_t Settings_GetSaturation(){
	return settings.saturation;
}

uint8_t Settings_GetEffect(){
	return settings.effect;
}

uint8_t Settings_GetEffectSpeed(){
	return settings.effectSpeed;
}

uint8_t Settings_GetColorMusic(){
	return settings.colorMusic;
}

uint8_t Settings_GetColorMusicSpeed(){
	return settings.colorMusicSpeed;
}

uint8_t SettingsGetUpdateFlag(){
	return settingsControl.updateFlag;
}