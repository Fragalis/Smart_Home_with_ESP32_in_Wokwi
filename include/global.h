#ifndef GLOBAL_H_
#define GLOBAL_H_

#define BOARD_NAME "ESP32_SMART_HOUSE_IN_WOKWI"
#define VERSION "1.1.0"

// Global libraries
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/gpio.h>
#include "esp_adc/adc_oneshot.h"
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
#include "lcdgfx.h"
#include "lcdgfx_gui.h"

// Configuration headers
#include "../src/config/hardware_config.h"
#include "../src/config/data_config.h"

// Device tasks headers
#include "../src/tasks/control_task.h"
#include "../src/tasks/dht22_task.h"
#include "../src/tasks/ldr_task.h"
#include "../src/tasks/lcd_task.h"
#include "../src/tasks/hc_sr04_task.h"

// Connection tasks headers
#include "../src/tasks/ntp_task.h"
#include "../src/tasks/wifi_task.h"
#include "../src/tasks/thingsboard_task.h"

// Utils headers
#include "../src/utils/utils.h"

// GPIO configuration
// Smart house output pins
#define LIGHT_PIN               GPIO_NUM_20
// DHT22 pin
#define DHT_PIN                 GPIO_NUM_47
// LDR pins
#define LDR_DO_PIN              GPIO_NUM_21     // Not used
#define LDR_AO_PIN              GPIO_NUM_10
// Stepper motor pins
#define STEPPER_DIRECTION_PIN   GPIO_NUM_21
#define STEPPER_STEP_PIN        GPIO_NUM_11
// ILI9341 pins
#define LCD_CS_PIN              GPIO_NUM_15
#define LCD_DC_PIN              GPIO_NUM_12
#define LCD_MOSI_PIN            GPIO_NUM_13
#define LCD_SCK_PIN             GPIO_NUM_9
#define LCD_LED_PIN             GPIO_NUM_17
#define LCD_MISO_PIN            GPIO_NUM_14
#define LCD_RST_PIN             GPIO_NUM_16
// HC-SR04 pins
#define HC_SR04_TRIG_PIN        GPIO_NUM_41
#define HC_SR04_ECHO_PIN        GPIO_NUM_42

// Timer configuration
#define DATA_SEMAPHORE_TIMER    100u
#define BLINK_LED_TIMER         1000u
#define DHT22_DELAY_TIMER       2500u
#define LDR_TIMER               5000u
#define WIFI_DELAY_TIMER        1000u
#define THINGSBOARD_DELAY_TIMER 1000u
#define THINGSBOARD_LOOP_TIMER  100u
#define NTP_WAIT_TIMER          1000u
#define NTP_SYNC_TIMER          15000u
#define STEPPER_DELAY_TIMER     10u
#define CHECK_STATE_TIMER       1000u
#define CONTROL_TIMER           1000u
#define LCD_FLUSH_TIMER         10u
#define LCD_DISPLAY_TIMER       1000u
#define LCD_FETCH_TIMER         5000u
#define HC_SR04_DELAY_TIMER     5000u

// LCD configuration
#define LCD_SPI_HOST            SPI1_HOST 
#define LCD_CLOCK_SPEED_HZ      10*1000*1000u
#define LCD_BUFFER_SIZE         100u

// Threshold configuration
#define LUMINOSITY_THRESHOLD    100u
#define SLEEP_TIME_THRESHOLD    23u
#define WAKE_TIME_THRESHOLD     7u
#define STEPPER_MAX_STEP        50u
#define DISTANCE_THRESHOLD      50u
#define DOOR_TIMEOUT            3u
#define SENSOR_INIT_TIMEOUT_MS  5000u

// ThingsBoard configuration
#define THINGSBOARD_SERVER      "app.coreiot.io"
#define TOKEN                   "wlp3f4coe12qet5k2zgs"
#define THINGSBOARD_PORT        1883u

// Data configuration
#define TEMPERATURE_NAN         -9999.0f   // Temperature cannot be below -273.15C
#define HUMIDITY_NAN            -9999.0f   // Humidity cannot be below 0%
#define LUMINOSITY_NAN          -1         // Luminosity cannot be negative
#define DATE_TIME_NAN           -1         // Timer cannot be negative
#define DISTANCE_NAN            -1         // Distance cannot be negative

#endif // GLOBAL_H_