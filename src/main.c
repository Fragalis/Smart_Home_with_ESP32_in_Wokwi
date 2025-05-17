#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/adc.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <mqtt_client.h>
#include <string.h>

void blink_led_task(void *arg) {
    ESP_LOGI("LED", "Starting LED blink task");
    while (1) {
        ESP_LOGI("LED", "Blinking LED");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
    }
}

void app_main() {
    xTaskCreate(blink_led_task, "blink_led_task", 2048, NULL, 5, NULL);
}