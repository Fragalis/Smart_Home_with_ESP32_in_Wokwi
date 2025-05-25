#include "control_task.h"

static const char *TAG = "CONTROL";

volatile bool light_state = false;
volatile bool door_state = false;

bool is_person_at_door() {
    if (data_storage.get_hc_sr04_data().distance < DISTANCE_THRESHOLD) return 1;
    return 0;
}

bool is_sleep_time() {
    if (data_storage.get_ntp_data().hour < WAKE_TIME_THRESHOLD) return 1;
    if (data_storage.get_ntp_data().hour > SLEEP_TIME_THRESHOLD) return 1;
    return 0;
}

bool is_dark() {
    if (data_storage.get_ldr_data().luminosity < LUMI_THRESHOLD) return 1;
    return 0;
}

void check_state_task(void *args) {
    while (1) {
        // If it's not sleep time (11PM - 7AM) and it's dark, turn on light
        light_state = !is_sleep_time() && is_dark();

        // If it's a person outside, open door
        // Might use a padlock for authentication in the future
        door_state = is_person_at_door();
        vTaskDelay(pdMS_TO_TICKS(CHECK_STATE_TIMER));
    }
}

void control_light(int state) {
    gpio_set_level(LIGHT_PIN, state); // Set lamp state
}

void control_door(int state) {
    gpio_set_level(STEPPER_DIRECTION_PIN, state);
    int step_count = 0;
    while (step_count < STEPPER_MAX_STEP) {
        step_count++;
        gpio_set_level(STEPPER_STEP_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(STEPPER_DELAY_TIMER));
        gpio_set_level(STEPPER_STEP_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(STEPPER_DELAY_TIMER));
    }
}

void control_task(void *arg) {
    int door_timer = 0;
    bool last_door_state = 0;
    while (1) {
        control_light(light_state);
        if (door_timer >= DOOR_TIMEOUT && door_state != last_door_state) {
            control_door(door_state);
            door_timer = 0;
            last_door_state = door_state;
        }
        door_timer = (door_timer >= DOOR_TIMEOUT)? DOOR_TIMEOUT : door_timer + 1;
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