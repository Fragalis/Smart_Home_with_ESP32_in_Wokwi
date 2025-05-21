#include "ldr_task.h"

static const char *TAG = "LDR";
static const int BUFFER_SIZE = 32;
static const float GAMMA = 0.7;
static const float RL10 = 50;

static uint32_t luminosity = 0;

void ldr_task(void *arg) 
{
    while (1) {
        // Read the LDR value from ADC
        int val = 0;
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, adc_chan, &val));
        float voltage = val / 4096.0 * 5;
        float resistance = 2000 * voltage / (1 - voltage / 5);
        luminosity = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA)) + 0.49999;

        // Store data
        data_storage.set_ldr_data(luminosity);
        
        // ESP_LOGI(TAG, "Luminosity: %lu", luminosity);
        // char json[BUFFER_SIZE];
        // snprintf(json, BUFFER_SIZE, "{\"lumi\": %d, \"is_dark\": %d}", ldr_data.luminosity, ldr_data.is_dark);
        // send_telemetry(json);
        vTaskDelay(pdMS_TO_TICKS(LDR_TIMER));
    }
}

void ldr_task_init(void)
{
    // Create the DHT22 task
    BaseType_t task_created = xTaskCreate(ldr_task, "ldr_task", 4096, NULL, 1, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create ldr_task");
        return;
    }
    ESP_LOGI(TAG, "ldr_task initialized");
}