#ifndef GLOBAL_H_
#define GLOBAL_H_

// Global libraries
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/gpio.h>
#include "driver/spi_master.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
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
#include "../src/tasks/lcd_task.h"
// Connection tasks headers
#include "../src/tasks/ntp_task.h"
#include "../src/tasks/wifi_task.h"
#include "../src/tasks/thingsboard_task.h"

// GPIO configuration
// LED and light pins
#define LED_PIN                 GPIO_NUM_19
#define LIGHT_PIN               GPIO_NUM_20
// DHT22 pin
#define DHT_PIN                 GPIO_NUM_47
// LDR pins
#define LDR_DO_PIN              GPIO_NUM_21
#define LDR_AO_PIN              GPIO_NUM_10
// Stepper motor pins
#define STEPPER_DIRECTION_PIN   GPIO_NUM_21
#define STEPPER_STEP_PIN        GPIO_NUM_11
// ILI9341 SPI pins
#define LCD_MOSI_PIN            GPIO_NUM_13
#define LCD_MISO_PIN            GPIO_NUM_14
#define LCD_SCLK_PIN            GPIO_NUM_9
#define LCD_CS_PIN              GPIO_NUM_15
#define LCD_DC_PIN              GPIO_NUM_12
#define LCD_RESET_PIN           GPIO_NUM_16
#define LCD_LED_PIN             GPIO_NUM_17

// Timer configuration
#define BLINK_LED_TIMER         1000u
#define DHT22_SEND_TIMER        2000u
#define DHT22_READ_TIMER        2000u
#define DHT22_DELAY_TIMER       5000u
#define LDR_TIMER               5000u
#define WIFI_DELAY_TIMER        1000u
#define THINGSBOARD_DELAY_TIMER 1000u
#define THINGSBOARD_LOOP_TIMER  100u
#define NTP_WAIT_TIMER          1000u
#define NTP_SYNC_TIMER          15000u
#define STEPPER_DELAY_TIMER     10u
#define CONTROL_TIMER           1000u

// LCD configuration
#define LCD_HOST                SPI2_HOST
#define LCD_H_RES               240
#define LCD_V_RES               320
#define LCD_PIXEL_CLOCK_HZ      (20 * 1000 * 1000)
#define LCD_CMD_BITS            8
#define LCD_PARAM_BITS          8
#define LCD_LIGHT_ON_LEVEL      1
#define LCD_DRAW_BUFFER_LINES   20 // number of display lines in each draw buffer

// Threshold configuration
#define LIGHT_THRESHOLD         100     // Threshold to toggle the lamp
#define MAX_STEPS               500     // Maximum steps for the stepper motor

#endif // GLOBAL_H_