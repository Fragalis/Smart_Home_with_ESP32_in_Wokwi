#include <global.h>

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
        return;
    }
    ESP_LOGI("GPIO", "GPIO initialized");
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

    // Initialize the LED blink task
    blink_led_task_init();
}

/*
    Sensors: DHT22 (temperature, humidity), LDR (light).
    Actuator: Relay (lamp control).
    Software: ESP-IDF, FreeRTOS tasks, ThingsBoard Client SDK for MQTT, Wokwi simulation.Ư
    Features: Telemetry publishing, RPC relay control, planned OTA.
*/