#include "control_task.h"

static const char *TAG = "CONTROL";

void control_stepper_motor(int state) {
    gpio_set_level(STEPPER_DIRECTION_PIN, state);
    int step_count = 0;
    while (step_count < MAX_STEPS) {
        step_count++;
        gpio_set_level(STEPPER_STEP_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(STEPPER_DELAY_TIMER));
        gpio_set_level(STEPPER_STEP_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(STEPPER_DELAY_TIMER));
    }
}

void control_light(int state) {
    gpio_set_level(LIGHT_PIN, state); // Set lamp state
}

enum CURTAIN_STATE {
    OPEN, OPENING,
    CLOSE, CLOSING
};

void control_task(void *arg) {
    CURTAIN_STATE curtain_state = OPEN; // Initial state
    local_data_t ldr_data, ntp_data;
    int luminosity = 1;
    int hour = 0;
    while (1) {
        // Get data
        if (ntp_queue != NULL && xQueueReceive(ntp_queue, &ntp_data, 0) == pdTRUE) {
            hour = ntp_data.data.ntp_data.hour; // Get current hour
            ESP_LOGI(TAG, "Current hour: %d", hour);
        }
        if (ldr_queue != NULL && xQueueReceive(ldr_queue, &ldr_data, 0) == pdTRUE) {
            luminosity = ldr_data.data.ldr_data.luminosity; // Get luminosity
            ESP_LOGI(TAG, "Luminosity: %d", luminosity);
        }
        switch (curtain_state) {
            case OPEN:
                // Close the curtain if it's dark or after 5 PM
                if (luminosity <= LIGHT_THRESHOLD || hour >= 17) {
                    ESP_LOGI(TAG, "Curtain is closing");
                    curtain_state = CLOSING;
                }
                break;
            case CLOSING:
                // Close the curtain
                control_stepper_motor(1);
                // Turn on lamp
                control_light(1);
                ESP_LOGI(TAG, "Curtain is closed");
                curtain_state = CLOSE;
                break;
            case CLOSE:
                // Open the curtain if it's light or between 7 AM and 12 PM
                if (luminosity > LIGHT_THRESHOLD || (hour >= 7 && hour <= 12)) {
                    ESP_LOGI(TAG, "Curtain is opening");
                    curtain_state = OPENING;
                }
                break;
            case OPENING:
                // Open the curtain
                control_stepper_motor(0);
                // Turn off lamp
                control_light(0);
                ESP_LOGI(TAG, "Curtain is opened");
                curtain_state = OPEN;
                break;
            default:
                break;
        }
        // ESP_LOGI(TAG, "Current curtain state: %d", curtain_state);
        vTaskDelay(pdMS_TO_TICKS(CONTROL_TIMER)); // Adjust delay as needed
    }
}

void control_task_init(void) 
{
    // Create the control task
    BaseType_t task_created = xTaskCreate(control_task, "control_task", 4096, NULL, 5, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create control_task");
        return;
    }
    ESP_LOGI(TAG, "control_task initialized");
}