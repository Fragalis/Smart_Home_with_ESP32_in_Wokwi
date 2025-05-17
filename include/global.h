#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include "../src/tasks/blink_led_task.h"

// GPIO configuration
#define LED_GPIO GPIO_NUM_19

#endif // GLOBAL_H_