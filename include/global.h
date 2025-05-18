#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>
#include "esp_system.h"
#include "esp_wifi.h"
#include <esp_timer.h>
#include <nvs_flash.h>

#include "../src/tasks/wifi_task.h"
#include "../src/tasks/blink_led_task.h"
#include "../src/tasks/dht22_task.h"

// GPIO configuration
#define LED_PIN GPIO_NUM_19

// DHT22 pin
#define DHT_PIN GPIO_NUM_47

#endif // GLOBAL_H_