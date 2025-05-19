#include "ntp_task.h"

static const char *TAG = "NTP";

void ntp_task(void *arg) {
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(NTP_TIMER));
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