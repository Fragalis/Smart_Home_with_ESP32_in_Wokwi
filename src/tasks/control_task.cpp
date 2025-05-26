#include "control_task.h"

static const char *TAG = "CONTROL";

volatile bool light_state = false;
volatile bool door_state = false;

bool is_person_at_door() {
    int16_t distance = data_storage.get_hc_sr04_data().distance;
    // Log the distance value for debugging
    // ESP_LOGI(TAG, "Distance: %d cm", distance);
    // Check if distance data is valid
    if (distance == DISTANCE_NAN) {
        ESP_LOGE(TAG, "HC-SR04 data is not available");
        return door_state; // Default to last door_state if HC-SR04 data is not available
    }
    return distance < DISTANCE_THRESHOLD;
}

bool is_dark() {
    int16_t luminosity = data_storage.get_ldr_data().luminosity;
    // Log the luminosity value for debugging
    // ESP_LOGI(TAG, "Luminosity: %d", luminosity);
    // Check if luminosity data is valid
    if (luminosity == LUMINOSITY_NAN) {
        ESP_LOGE(TAG, "LDR data is not available");
        return light_state; // Default to last light_state if LDR data is not available
    }
    return luminosity < LUMINOSITY_THRESHOLD;
}

void check_state_task(void *args) {
    while (1) {
        // If it's dark, turn on light
        light_state = is_dark();

        // If it's a person outside, open door
        // Might use a padlock for authentication in the future
        door_state = is_person_at_door();
        vTaskDelay(pdMS_TO_TICKS(CHECK_STATE_TIMER));
    }
}

void control_light(int state) {
    gpio_set_level(LIGHT_PIN, state); // Set lamp state
}

void control_door(void *args) {
    int state = *(int*)args; // Get the state from the argument
    gpio_set_level(STEPPER_DIRECTION_PIN, state);
    int step_count = 0;
    while (step_count < STEPPER_MAX_STEP) {
        gpio_set_level(STEPPER_STEP_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(STEPPER_DELAY_TIMER));
        gpio_set_level(STEPPER_STEP_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(STEPPER_DELAY_TIMER));
        step_count++;
    }
    ESP_LOGI(TAG, "control_door task terminated");
    vTaskDelete(NULL); // Delete the task after completion
}

typedef enum {
    DOOR_IDLE, DOOR_FUNCTION
} door_state_e;

void control_task(void *arg) {
    int door_timer = 0;
    bool last_door_state = 0;
    while (1) {
        control_light(light_state);
        if (door_timer >= DOOR_TIMEOUT && door_state != last_door_state) {
            xTaskCreate(control_door, "control_door", 1024, (void*)&door_state, 5, NULL);
            ESP_LOGI(TAG, "control_door task created");
            door_timer = 0;
            last_door_state = door_state;
        }
        door_timer = (door_timer >= DOOR_TIMEOUT)? DOOR_TIMEOUT : door_timer + 1;
        // ESP_LOGI(TAG, "Door state: %d", door_state);
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
    BaseType_t control_task_created = xTaskCreate(control_task, "control_task", 4096, NULL, 4, NULL);
    if (control_task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create control_task");
        return;
    }
    ESP_LOGI(TAG, "control_task initialized");
}