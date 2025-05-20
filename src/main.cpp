#include <global.h>

extern "C" {
    void app_main(void) {
        // Setup configurations
        hardware_config_init();

        // Setup device tasks
        blink_led_task_init();
        dht22_task_init();
        ldr_task_init();
        control_task_init();
        lcd_task_init();
        
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