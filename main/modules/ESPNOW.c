#include "ESPNOW.h"

static uint8_t _channel = 0;
static uint8_t _interface = ESPNOW_IF_WIFI_STA;
volatile static uint8_t sendStatus = ESPNOW_SEND_STATUS_UNKNOWN;

uint8_t receivedFlag = 0;
uint8_t receivedData[250] = {0};
size_t receivedSize = 0; 
uint8_t receivedSenderAddress[MAC_SIZE] = {0};
uint8_t receivedReceiverAddress[MAC_SIZE] = {0};
int receivedRssi = 0;

//-----Public Functions------
void ESPNOW_Init(uint8_t channel, ESPNOW_Interface_et interface){
	_channel = channel;
	_interface = interface;
    esp_now_init();
    esp_now_register_recv_cb(receive_callback);
	esp_now_register_send_cb(send_callback);
}

void ESPNOW_Run(){
	if(receivedFlag){
		ESPNOW_ReceiveCallback(receivedData, receivedSize, receivedSenderAddress, receivedReceiverAddress, receivedRssi);
		receivedFlag = 0;
	}
}

ESPNOW_SendStatus_et ESPNOW_Send(uint8_t* data, size_t size, uint8_t* receiverAddress, ESPNOW_WaitSendStatus_et waitSendStatus){
	esp_now_peer_info_t esp_now_peer_info = {};
	memcpy(esp_now_peer_info.peer_addr, receiverAddress, MAC_SIZE);
	esp_now_peer_info.channel = _channel;
	esp_now_peer_info.ifidx = _interface;

	esp_now_add_peer(&esp_now_peer_info);
	esp_now_send(receiverAddress, data, size);
	esp_now_del_peer(receiverAddress);

	sendStatus = ESPNOW_SEND_STATUS_UNKNOWN;

	uint64_t lastTime = esp_timer_get_time();
	if(waitSendStatus == ESPNOW_WAIT_SEND_STATUS){
		while(sendStatus == ESPNOW_SEND_STATUS_UNKNOWN && esp_timer_get_time() - lastTime < WAIT_TIME);
		if(sendStatus == ESPNOW_SEND_STATUS_UNKNOWN && esp_timer_get_time() - lastTime >= WAIT_TIME){
			sendStatus = ESPNOW_SEND_STATUS_TIMEOUT;
		}
	}

	return sendStatus;
}

//-----Private Functions------
void send_callback(const uint8_t* mac_addr, esp_now_send_status_t status){
	switch(status){
		case ESP_NOW_SEND_SUCCESS: sendStatus = ESPNOW_SEND_STATUS_SUCCESS; break;
		case ESP_NOW_SEND_FAIL:    sendStatus = ESPNOW_SEND_STATUS_FAIL;    break;
	}
	ESPNOW_SendCallback(mac_addr, sendStatus);
}

void receive_callback(const struct esp_now_recv_info* recv_info, const uint8_t *data, int data_len){
	if(!receivedFlag){
		memcpy(receivedData, data, data_len);
		memcpy(receivedSenderAddress, recv_info->src_addr, MAC_SIZE);
		memcpy(receivedReceiverAddress, recv_info->des_addr, MAC_SIZE);
		receivedSize = data_len;
		receivedRssi = recv_info->rx_ctrl->rssi;
		receivedFlag = 1;		
	}

}

//-----Weak Functions------
__attribute((weak)) void ESPNOW_SendCallback(const uint8_t* receiverAddress, ESPNOW_SendStatus_et status){};
__attribute((weak)) void ESPNOW_ReceiveCallback(const uint8_t* data, size_t size, uint8_t* senderAddress, uint8_t* receiverAddress, int rssi){};