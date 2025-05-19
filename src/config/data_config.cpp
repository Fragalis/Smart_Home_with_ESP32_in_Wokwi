#include "data_config.h"

static const char *TAG = "DATA";

QueueHandle_t dht22_queue;
QueueHandle_t ldr_queue;
QueueHandle_t ntp_queue;

void data_config_task(void *arg) {
    // local_data_t data;
    // while (1) {
    //     // Wait for data from the queues
    //     if (dht22_queue != NULL && xQueueReceive(dht22_queue, &data, 0) == pdTRUE) {
    //         ESP_LOGI(TAG, "DHT22 Data: Temp: %.2f, Humidity: %.2f", 
    //                       data.data.dht22_data.temperature, 
    //                       data.data.dht22_data.humidity
    //                 );
    //     }
    //     if (ldr_queue != NULL && xQueueReceive(ldr_queue, &data, 0) == pdTRUE) {
    //         ESP_LOGI(TAG, "LDR Data: Luminosity: %d, Is Dark: %d", 
    //                       data.data.ldr_data.luminosity, 
    //                       data.data.ldr_data.is_dark
    //                 );
    //     }
    //     if (ntp_queue != NULL && xQueueReceive(ntp_queue, &data, 0) == pdTRUE) {
    //         ESP_LOGI(TAG, "NTP Data: Timestamp: %lld", 
    //                       data.data.ntp_data.timestamp
    //                 );
    //     }
    //     vTaskDelay(pdMS_TO_TICKS(1000)); // Adjust delay as needed
    // }
}

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
    
    ESP_LOGI(TAG, "Data configuration initialized");

    // // Create the data config task
    // BaseType_t task_created = xTaskCreate(data_config_task, "data_config_task", 4096, NULL, 5, NULL);
    // if (task_created != pdPASS) {
    //     ESP_LOGE(TAG, "Failed to create data_config_task");
    //     return;
    // }
    // ESP_LOGI(TAG, "data_config_task initialized");
}