#include "control_task.h"

static const char *TAG = "CONTROL";

volatile bool light_state = false;

bool is_dark() {
    if (data_storage.get_ldr_data().luminosity < LIGHT_THRESHOLD) return 1;
    return 0;
}

void check_state_task(void *args) {
    while (1) {
        light_state = is_dark();
        vTaskDelay(pdMS_TO_TICKS(CHECK_STATE_TIMER));
    }
}

void control_light(int state) {
    gpio_set_level(LIGHT_PIN, state); // Set lamp state
}

enum DOOR_STATE {
    OPEN, OPENING,
    CLOSE, CLOSING
};

void control_task(void *arg) {
    while (1) {
        control_light(light_state);
        vTaskDelay(pdMS_TO_TICKS(CONTROL_TIMER)); // Adjust delay as needed
    }
}

void control_task_init(void) 
{
    // Create the control task
    BaseType_t check_state_task_created = xTaskCreate(check_state_task, "check_state_task", 4096, NULL, 3, NULL);
    if (check_state_task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create check_state_task");
        return;
    }
    ESP_LOGI(TAG, "check_state_task initialized");

    // Create the control task
    BaseType_t control_task_created = xTaskCreate(control_task, "control_task", 2048, NULL, 4, NULL);
    if (control_task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create control_task");
        return;
    }
    ESP_LOGI(TAG, "control_task initialized");
}