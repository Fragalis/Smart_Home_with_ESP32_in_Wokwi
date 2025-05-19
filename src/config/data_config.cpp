#include "data_config.h"

static const char *TAG = "DATA";

QueueHandle_t dht22_queue;
QueueHandle_t ldr_queue;
QueueHandle_t ntp_queue;

void data_config_init(void) {
    // Initialize the queues for DHT22, LDR, and NTP tasks
    dht22_queue = xQueueCreate(10, sizeof(local_data_t));
    if (dht22_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create dht22_queue");
        return;
    }
    ESP_LOGI(TAG, "dht22_queue initialized");

    ldr_queue = xQueueCreate(10, sizeof(local_data_t));
    if (ldr_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create ldr_queue");
        return;
    }
    ESP_LOGI(TAG, "ldr_queue initialized");

    ntp_queue = xQueueCreate(1, sizeof(local_data_t));
    if (ntp_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create ntp_queue");
        return;
    }
    ESP_LOGI(TAG, "ntp_queue initialized");
}