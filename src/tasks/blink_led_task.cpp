#include "blink_led_task.h"

static const char *TAG = "LED";

static void init_gpio(void) {
    esp_err_t ret;
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ret = gpio_config(&io_conf);
    if (ret != ESP_OK) {
        ESP_LOGE("GPIO", "Failed to configure GPIO %d: %s", LED_PIN, esp_err_to_name(ret));
        return;
    }
    ESP_LOGI("GPIO", "GPIO initialized");
}

static void blink_led_task(void *arg) {
    bool led_state = false;
    
    while (1) {
        led_state = !led_state;
        esp_err_t ret = gpio_set_level(LED_PIN, led_state);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to set GPIO %d: %s", LED_PIN, esp_err_to_name(ret));
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Blink every 1 second
    }
}

void blink_led_task_init() {
    // Initialize GPIO
    init_gpio();
    
    // Create the LED blink task
    BaseType_t task_created = xTaskCreate(blink_led_task, "blink_led_task", 2048, NULL, 5, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create blink_led_task");
        return;
    }
    ESP_LOGI(TAG, "blink_led_task initialized");
}