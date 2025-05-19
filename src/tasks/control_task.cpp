#include "control_task.h"

#define STEPPER_DIRECTION_PIN GPIO_NUM_47
#define STEPPER_STEP_PIN GPIO_NUM_11

void control_task(void *arg) {
    while (1) {
        // Control the stepper motor
        gpio_set_level(STEPPER_DIRECTION_PIN, 1); // Set direction
        gpio_set_level(STEPPER_STEP_PIN, 1);      // Step
        vTaskDelay(pdMS_TO_TICKS(100));           // Delay for step duration
        gpio_set_level(STEPPER_STEP_PIN, 0);      // Reset step
        vTaskDelay(pdMS_TO_TICKS(100));           // Delay for step duration
        // Add more control logic as needed
    }
}

void control_task_init(void) 
{
    // Initialize GPIO for stepper motor control
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << STEPPER_DIRECTION_PIN) | (1ULL << STEPPER_STEP_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_LOGI("CONTROL", "Stepper motor control initialized");

    // Create the control task
    BaseType_t task_created = xTaskCreate(control_task, "control_task", 4096, NULL, 5, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE("CONTROL", "Failed to create control_task");
        return;
    }
    ESP_LOGI("CONTROL", "control_task initialized");
}