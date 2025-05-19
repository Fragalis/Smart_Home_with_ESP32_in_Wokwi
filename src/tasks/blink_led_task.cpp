#include "blink_led_task.h"

static const char *TAG = "LED";

static void blink_led_task(void *arg) 
{
    bool led_state = false;
    while (1) {
        led_state = !led_state;
        esp_err_t ret = gpio_set_level(LED_PIN, led_state);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to set GPIO %d: %s", LED_PIN, esp_err_to_name(ret));
        }
        vTaskDelay(pdMS_TO_TICKS(BLINK_LED_TIMER)); // Blink every 1 second
    }
}

void blink_led_task_init() 
{
    // Create the LED blink task
    BaseType_t task_created = xTaskCreate(blink_led_task, "blink_led_task", 2048, NULL, 5, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create blink_led_task");
        return;
    }
    ESP_LOGI(TAG, "blink_led_task initialized");
}