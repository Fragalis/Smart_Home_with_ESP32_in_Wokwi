#include "hc_sr04_task.h"

static const char* TAG = "HC-SR04";

void hc_sr04_task(void *args) {
    while (1) {
        // Start measurement by setting TRIG pin to 1 in 10us
        gpio_set_level(HC_SR04_TRIG_PIN, 1);
        esp_rom_delay_us(10); 
        gpio_set_level(HC_SR04_TRIG_PIN, 0); 

        // Await the next high signal from ECHO pin
        if (await_signal(HC_SR04_ECHO_PIN, 100, 1) == 0) {
            ESP_LOGE(TAG, "Unable to receive data from sensor");
            vTaskDelay(pdMS_TO_TICKS(HC_SR04_DELAY_TIMER));
            continue;
        }

        uint16_t timer = await_signal(HC_SR04_ECHO_PIN, UINT16_MAX, 0);
        if (timer == 0) {
            ESP_LOGE(TAG, "Request timeout");
            vTaskDelay(pdMS_TO_TICKS(HC_SR04_DELAY_TIMER));
            continue;
        }

        uint16_t distance = timer / 12;
        ESP_LOGI(TAG, "distance %ucm", distance);
        vTaskDelay(pdMS_TO_TICKS(HC_SR04_DELAY_TIMER));
    }
}

void hc_sr04_task_init() {
    // Create the HC-SR04 task
    BaseType_t task_created = xTaskCreate(hc_sr04_task, "dht2hc_sr04_task2_task", 2048, NULL, 1, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create hc_sr04_task");
        return;
    }
    ESP_LOGI(TAG, "hc_sr04_task initialized");
}