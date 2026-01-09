#include "spiffs.h"

void SPIFFS_Init(char* path){
	esp_vfs_spiffs_conf_t config = {
			.base_path = path,
			.partition_label = NULL,
			.max_files = 5,
			.format_if_mount_failed = true
	};

	esp_vfs_spiffs_register(&config);	
}

size_t SPIFFS_ReadFile(char* path, uint8_t* data, size_t size){
	FILE *file = fopen(path, "r");
	size_t count = fread(data, sizeof(uint8_t), size, file);
	fclose(file);
	return count;	
}

size_t SPIFFS_WriteFile(char* path, uint8_t* data, size_t size){
	FILE *file = fopen(path, "w");
	size_t count = fwrite(data, sizeof(uint8_t), size, file);
	fclose(file);
	return count;	
}