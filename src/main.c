#include <global.h>

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize the LED blink task
    blink_led_task_init();

    // Initialize the DHT22 task
    dht22_task_init();

    // Initialize the WiFi task
    wifi_task_init();
}

/*
    Sensors: DHT22 (temperature, humidity), LDR (light).
    Actuator: Relay (lamp control).
    Software: ESP-IDF, FreeRTOS tasks, ThingsBoard Client SDK for MQTT, Wokwi simulation.Ư
    Features: Telemetry publishing, RPC relay control, planned OTA.
*/