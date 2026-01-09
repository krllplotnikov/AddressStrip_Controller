#ifndef ESPNOW_H_
#define ESPNOW_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_now.h"
#include "esp_timer.h"
#include "esp_log.h"

#define MAC_SIZE 6
#define WAIT_TIME 50000

typedef enum {
    ESPNOW_IF_WIFI_STA,
    ESPNOW_IF_WIFI_AP,
} ESPNOW_Interface_et;

typedef enum{
	ESPNOW_SEND_STATUS_UNKNOWN,
	ESPNOW_SEND_STATUS_TIMEOUT,
	ESPNOW_SEND_STATUS_SUCCESS,
	ESPNOW_SEND_STATUS_FAIL 
}ESPNOW_SendStatus_et;

typedef enum{
	ESPNOW_WAIT_SEND_STATUS,
	ESPNOW_NOT_WAIT_SEND_STATUS
}ESPNOW_WaitSendStatus_et;

typedef void (*SendCallback)(const uint8_t* receiverAddress, ESPNOW_SendStatus_et status);
typedef void (*ReceiveCallback)(const uint8_t* data, size_t size, uint8_t* senderAddress, int rssi);

void ESPNOW_Init(uint8_t channel, ESPNOW_Interface_et interface);
void ESPNOW_Run();
ESPNOW_SendStatus_et ESPNOW_Send(uint8_t* data, size_t size, uint8_t* receiverAddress, ESPNOW_WaitSendStatus_et waitSendStatus);
void ESPNOW_SendCallback(const uint8_t* receiverAddress, ESPNOW_SendStatus_et status);
void ESPNOW_ReceiveCallback(const uint8_t* data, size_t size, uint8_t* senderAddress, uint8_t* receiverAddress, int rssi);

void send_callback(const uint8_t * mac_addr, esp_now_send_status_t status);
void receive_callback(const struct esp_now_recv_info* recv_info, const uint8_t *data, int data_len);

#endif