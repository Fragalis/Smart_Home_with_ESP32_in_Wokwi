#include "ntp_task.h"
#include "time.h"

static const char *TAG = "NTP";

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
    
    struct tm timeinfo;
    time_t now;
    while (1) {
        time(&now);
        localtime_r(&now, &timeinfo);

        // Store data
        data_storage.set_ntp_data(timeinfo.tm_min, timeinfo.tm_hour, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year + 1900);

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