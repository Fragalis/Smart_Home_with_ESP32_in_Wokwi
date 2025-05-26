#include <global.h>

extern "C" {


    void app_main(void) {
        // Setup configurations
        hardware_config_init();

        // Setup device tasks
        dht22_task_init();
        ldr_task_init();
        hc_sr04_task_init();
        
        TickType_t start_time = xTaskGetTickCount();
        while ((xTaskGetTickCount() - start_time) < pdMS_TO_TICKS(SENSOR_INIT_TIMEOUT_MS)) {
            if (data_storage.is_sensor_data_valid()) {
                ESP_LOGI("MAIN", "All sensor data is valid");
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        
        // Setup control tasks
        if (data_storage.is_sensor_data_valid()) {
            control_task_init();
            lcd_task_init();
            ESP_LOGI("MAIN", "All data is valid, initializing connection tasks");
        } else {
            ESP_LOGE("MAIN", "Data is not valid, skipping connection tasks");
        }

        // Setup connection tasks
        wifi_task_init();
        thingsboard_task_init();
        ntp_task_init();
    }
}
/*
    Your current system, reading DHT22 data and pushing it to ThingsBoard via MQTT, is a solid foundation. 
    To emphasize UART and SPI protocols and learn multitasking, you can add:
    - Serial LCD Display (UART): Display DHT22 readings locally for immediate feedback, a common feature in smart homes.
    - SPI-Based Accelerometer (e.g., ADXL345): Detect motion for security or automation, relevant for smart home applications.
    - SD Card (SPI): Log sensor data for offline analysis, useful for troubleshooting or compliance.
*/