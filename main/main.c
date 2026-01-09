#include <stdio.h>

#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Periphery.h"

void AddressStrip_Task(){
	while(1){
		printf("Hello!\r\n");
		vTaskDelay(1000);
	}
}

void FFT_Task(){
	while(1){
		vTaskDelay(1);
	}
}

void app_main(void)
{
	RMT_Init();
	I2S_Init();
	
	NVS_Init();
	WiFi_Init();
	
	xTaskCreate(AddressStrip_Task, NULL, 8096, NULL, 0, NULL);
	xTaskCreate(FFT_Task, NULL, 8096 * 4, NULL, 20, NULL);
}
