#include "Periphery.h"

#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM      21

#define I2S_WS_GPIO_NUM 10
#define I2S_SD_GPIO_NUM 20
#define I2S_SCK_GPIO_NUM 9
#define I2S_PORT I2S_NUM_0

#define NUM_SAMPLES 256
#define SAMPLE_RATE 16000

rmt_encoder_handle_t simple_encoder = NULL;
rmt_channel_handle_t led_chan = NULL;

static const rmt_symbol_word_t ws2812_zero = {
    .level0 = 1,
    .duration0 = 0.3 * RMT_LED_STRIP_RESOLUTION_HZ / 1000000,
    .level1 = 0,
    .duration1 = 0.9 * RMT_LED_STRIP_RESOLUTION_HZ / 1000000,
};

static const rmt_symbol_word_t ws2812_one = {
    .level0 = 1,
    .duration0 = 0.9 * RMT_LED_STRIP_RESOLUTION_HZ / 1000000,
    .level1 = 0,
    .duration1 = 0.3 * RMT_LED_STRIP_RESOLUTION_HZ / 1000000,
};

static const rmt_symbol_word_t ws2812_reset = {
    .level0 = 1,
    .duration0 = RMT_LED_STRIP_RESOLUTION_HZ / 1000000 * 50 / 2,
    .level1 = 0,
    .duration1 = RMT_LED_STRIP_RESOLUTION_HZ / 1000000 * 50 / 2,
};

rmt_tx_channel_config_t tx_chan_config = {
    .clk_src = RMT_CLK_SRC_DEFAULT,
    .gpio_num = RMT_LED_STRIP_GPIO_NUM,
    .mem_block_symbols = 64,
    .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
    .trans_queue_depth = 4,
};

rmt_transmit_config_t tx_config = {
    .loop_count = 0,
};	
    
static size_t rmt_encoder_callback(const void *data, size_t data_size,
                               size_t symbols_written, size_t symbols_free,
                               rmt_symbol_word_t *symbols, bool *done, void *arg)
{
    if (symbols_free < 8) {
        return 0;
    }

    size_t data_pos = symbols_written / 8;
    uint8_t *data_bytes = (uint8_t*)data;
    if (data_pos < data_size) {
        size_t symbol_pos = 0;
        for (int bitmask = 0x80; bitmask != 0; bitmask >>= 1) {
            if (data_bytes[data_pos]&bitmask) {
                symbols[symbol_pos++] = ws2812_one;
            } else {
                symbols[symbol_pos++] = ws2812_zero;
            }
        }
        return symbol_pos;
    } else {
        symbols[0] = ws2812_reset;
        *done = 1;
        return 1;
    }
}

const rmt_simple_encoder_config_t simple_encoder_cfg = {
    .callback = rmt_encoder_callback
};




void RMT_Init(){
    rmt_new_tx_channel(&tx_chan_config, &led_chan);
    rmt_new_simple_encoder(&simple_encoder_cfg, &simple_encoder);
    rmt_enable(led_chan);
}

void I2S_Init(){
  const i2s_config_t i2s_config = {.mode = I2S_MODE_MASTER | I2S_MODE_RX,
                                   .sample_rate = SAMPLE_RATE,
                                   .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
                                   .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
                                   .communication_format =
                                       I2S_COMM_FORMAT_STAND_I2S,
                                   .intr_alloc_flags = 0,
                                   .dma_buf_count = 8,
                                   .dma_buf_len = NUM_SAMPLES,
                                   .use_apll = false};

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);	
  
  const i2s_pin_config_t pin_config = {.bck_io_num = I2S_SCK_GPIO_NUM,
                                       .ws_io_num = I2S_WS_GPIO_NUM,
                                       .data_out_num = -1,
                                       .data_in_num = I2S_SD_GPIO_NUM};

  i2s_set_pin(I2S_PORT, &pin_config);  
  
  i2s_start(I2S_PORT);
}

void NVS_Init(){
    nvs_flash_init();
}

void WiFi_Init(){
	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

	esp_netif_init();
	esp_event_loop_create_default();
    nvs_flash_init();
	esp_wifi_set_mode(WIFI_MODE_STA);
	esp_wifi_set_storage(WIFI_STORAGE_FLASH);
	esp_wifi_init(&wifi_init_config);
	esp_wifi_start();
}