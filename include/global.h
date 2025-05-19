#ifndef GLOBAL_H_
#define GLOBAL_H_

// Global libraries
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/gpio.h>
#include "esp_adc/adc_oneshot.h"
#include "driver/spi_common.h"
#include "esp_lcd_types.h"
#include "esp_lcd_panel_ops.h"
#include <esp_err.h>
#include <esp_log.h>
#include "esp_system.h"
#include "esp_wifi.h"
#include <esp_timer.h>
#include "esp_sntp.h"
#include <nvs_flash.h>
#include <math.h>   

// Library dependencies
#include "Espressif_MQTT_Client.h"
#include "ThingsBoard.h"

// Configuration headers
#include "../src/config/hardware_config.h"
#include "../src/config/data_config.h"

// Device tasks headers
#include "../src/tasks/control_task.h"
#include "../src/tasks/blink_led_task.h"
#include "../src/tasks/dht22_task.h"
#include "../src/tasks/ldr_task.h"
#include "../src/tasks/ili9341_task.h"
// Connection tasks headers
#include "../src/tasks/ntp_task.h"
#include "../src/tasks/wifi_task.h"
#include "../src/tasks/thingsboard_task.h"

// GPIO configuration
// LED and light pins
#define LED_PIN GPIO_NUM_19
#define LIGHT_PIN GPIO_NUM_20
// DHT22 pin
#define DHT_PIN GPIO_NUM_47
// LDR pins
#define LDR_DO_PIN GPIO_NUM_21
#define LDR_AO_PIN GPIO_NUM_10
// Stepper motor pins
#define STEPPER_DIRECTION_PIN GPIO_NUM_21
#define STEPPER_STEP_PIN GPIO_NUM_11
// ILI9341 SPI pins
#define SCK_PIN GPIO_NUM_9
#define DATA_PIN GPIO_NUM_12
#define MOSI_PIN GPIO_NUM_13
#define MISO_PIN GPIO_NUM_14
#define CS_PIN GPIO_NUM_15

// Timer configuration
#define BLINK_LED_TIMER 1000u
#define DHT22_SEND_TIMER 2000u
#define DHT22_READ_TIMER 2000u
#define DHT22_DELAY_TIMER 5000u
#define LDR_TIMER 5000u
#define WIFI_DELAY_TIMER 1000u
#define THINGSBOARD_DELAY_TIMER 1000u
#define THINGSBOARD_LOOP_TIMER 100u
#define NTP_WAIT_TIMER 1000u
#define NTP_SYNC_TIMER 15000u
#define STEPPER_DELAY_TIMER 10u
#define CONTROL_TIMER 1000u

// Threshold configuration
constexpr const int LIGHT_THRESHOLD = 100; // Threshold to toggle the lamp
constexpr const int MAX_STEPS = 500;       // Maximum steps for the stepper motor

#endif // GLOBAL_H_