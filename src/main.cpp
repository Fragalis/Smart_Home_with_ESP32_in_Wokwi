#include <global.h>

extern "C" {
    void app_main(void) {
        // Setup hardware pins
        hardware_init();

        // Initialize the LED blink task
        blink_led_task_init();

        // Initialize the DHT22 task
        dht22_task_init();

        // Initialize the LDR task
        ldr_task_init();

        // Initialize the WiFi task
        wifi_task_init();

        // Initialize the ThingsBoard task
        thingsboard_task_init();

        // Initialize the NTP task
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