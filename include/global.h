#ifndef GLOBAL_H_
#define GLOBAL_H_

// Global libraries
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "esp_adc/adc_oneshot.h"
#include <esp_err.h>
#include <esp_log.h>
#include "esp_system.h"
#include "esp_wifi.h"
#include <esp_timer.h>
#include <nvs_flash.h>
#include <math.h>
#include "Espressif_MQTT_Client.h"
#include "ThingsBoard.h"

// Tasks headers
#include "../src/tasks/hardware_init_task.h"
#include "../src/tasks/blink_led_task.h"
#include "../src/tasks/dht22_task.h"
#include "../src/tasks/ldr_task.h"
#include "../src/tasks/wifi_task.h"
#include "../src/tasks/thingsboard_task.h"

// GPIO configuration
#define LED_PIN GPIO_NUM_19
#define DHT_PIN GPIO_NUM_47
#define LDR_DO_PIN GPIO_NUM_21
#define LDR_AO_PIN GPIO_NUM_10

// Timer configuration
#define BLINK_LED_TIMER 1000 
#define DHT22_SEND_TIMER 2000 
#define DHT22_READ_TIMER 2000
#define DHT22_DELAY_TIMER 5000 
#define LDR_TIMER 5000
#define THINGSBOARD_DELAY_TIMER 1000
#define THINGSBOARD_LOOP_TIMER 100

#endif // GLOBAL_H_