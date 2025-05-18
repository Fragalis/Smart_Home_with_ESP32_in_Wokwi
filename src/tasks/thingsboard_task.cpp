#include "thingsboard_task.h"

#define BROKER_URL "app.coreiot.io"
#define TOKEN "wlp3f4coe12qet5k2zgs"
#define PORT 1883u

Espressif_MQTT_Client mqttClient;
ThingsBoard tb(mqttClient);
static const char *TAG = "TB";

void send_telemetry(const char *json) {
    tb.sendTelemetryString(json);
}

void thingsboard_task(void *arg) {
    while (!wifi_is_connected) {
        ESP_LOGE(TAG, "WiFi not connected");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    tb.connect(BROKER_URL, TOKEN, PORT);
    while (!tb.connected()) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "Connected");
    while (1) {
        tb.loop();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void thingsboard_task_init() {
    // Create the ThingsBoard task
    BaseType_t task_created = xTaskCreate(thingsboard_task, "thingsboard_task", 2048, NULL, 5, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create thingsboard_task");
        return;
    }
    ESP_LOGI(TAG, "thingsboard_task initialized");
}