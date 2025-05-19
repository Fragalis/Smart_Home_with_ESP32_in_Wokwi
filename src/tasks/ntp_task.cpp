#include "ntp_task.h"
#include "time.h"

static const char *TAG = "NTP";
QueueHandle_t ntp_queue;

void ntp_task(void *arg) {
    while (!wifi_is_connected) {
        ESP_LOGE(TAG, "WiFi not connected");
        vTaskDelay(pdMS_TO_TICKS(WIFI_DELAY_TIMER));
    }

    
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "vn.pool.ntp.org");
    esp_sntp_init();
    
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET) {
        ESP_LOGI(TAG, "Waiting for NTP sync...");
        vTaskDelay(pdMS_TO_TICKS(NTP_WAIT_TIMER));
    }
    
    setenv("TZ", "UTC-7", 1);
    tzset();
    
    local_data_t ntp_data;
    ntp_data.type = NTP;
    struct tm timeinfo;
    time_t now;
    while (1) {
        time(&now);
        localtime_r(&now, &timeinfo);
        ntp_data.data.ntp_data.timestamp = now;        
        if (ntp_queue != NULL && xQueueSend(ntp_queue, &ntp_data, 0) != pdPASS) {
            ESP_LOGE(TAG, "Failed to send NTP data to queue");
        }

        vTaskDelay(pdMS_TO_TICKS(NTP_SYNC_TIMER));
    }
}

void ntp_task_init() {
    // Create the NTP task
    BaseType_t task_created = xTaskCreate(ntp_task, "ntp_task", 4096, NULL, 5, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create ntp_task");
        return;
    }
    ESP_LOGI(TAG, "ntp_task initialized");
}