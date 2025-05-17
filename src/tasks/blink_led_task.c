#include "blink_led_task.h"

static void blink_led_task(void *arg) {
    bool led_state = false;
    
    while (1) {
        led_state = !led_state;
        esp_err_t ret = gpio_set_level(LED_GPIO, led_state);
        if (ret != ESP_OK) {
            ESP_LOGE("LED", "Failed to set GPIO %d: %s", LED_GPIO, esp_err_to_name(ret));
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Blink every 1 second
    }
}

void blink_led_task_init() {
    // Create the LED blink task
    BaseType_t task_created = xTaskCreate(blink_led_task, "blink_led_task", 2048, NULL, 5, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE("LED", "Failed to create blink_led_task");
        return;
    }
    ESP_LOGI("LED", "Blink LED task initialized");
}