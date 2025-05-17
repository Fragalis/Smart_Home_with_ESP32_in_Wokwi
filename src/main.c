#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>

// GPIO configuration
#define LED_GPIO GPIO_NUM_19

static void init_gpio(void) {
    esp_err_t ret;
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ret = gpio_config(&io_conf);
    if (ret != ESP_OK) {
        ESP_LOGE("GPIO", "Failed to configure GPIO %d: %s", LED_GPIO, esp_err_to_name(ret));
    }
}

static void blink_led_task(void *arg) {
    bool led_state = false;
    ESP_LOGI("LED", "Starting LED blink task");

    while (1) {
        led_state = !led_state;
        esp_err_t ret = gpio_set_level(LED_GPIO, led_state);
        if (ret != ESP_OK) {
            ESP_LOGE("LED", "Failed to set GPIO %d: %s", LED_GPIO, esp_err_to_name(ret));
        }
        ESP_LOGI("LED", "LED %s", led_state ? "ON" : "OFF");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Blink every 1 second
    }
}

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize GPIO
    init_gpio();

    // Create blink task
    BaseType_t task_created = xTaskCreate(blink_led_task, "blink_led_task", 2048, NULL, 5, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE("MAIN", "Failed to create blink_led_task");
    }
}