#include "ldr_task.h"

static const char *TAG = "LDR";
const float GAMMA = 0.7;
const float RL10 = 50;

// Structure for LDR data
typedef struct {
    int luminosity; // ADC value (0–4095)
    int is_dark;    // DO state (1 for dark)
} ldr_data_t;

void ldr_task(void *arg) 
{
    ldr_data_t ldr_data;
    while (1) {
        // Read the LDR value from ADC
        int val = 0;
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, adc_chan, &val));
        float voltage = val / 4096.0 * 5;
        float resistance = 2000 * voltage / (1 - voltage / 5);
        ldr_data.luminosity = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

        // Read the digital output (DO) state
        ldr_data.is_dark = gpio_get_level(LDR_DO_PIN);

        ESP_LOGI(TAG, "LDR Luminosity: %d, is dark: %d", ldr_data.luminosity, ldr_data.is_dark);
        char json[64];
        snprintf(json, 64, "{\"luminosity\": %d, \"is_dark\": %d}", ldr_data.luminosity, ldr_data.is_dark);
        send_telemetry(json);
        vTaskDelay(pdMS_TO_TICKS(LDR_TIMER));
    }
}

void ldr_task_init(void)
{
    // Create the DHT22 task
    BaseType_t task_created = xTaskCreate(ldr_task, "ldr_task", 4096, NULL, 5, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create ldr_task");
        return;
    }
    ESP_LOGI(TAG, "ldr_task initialized");
}