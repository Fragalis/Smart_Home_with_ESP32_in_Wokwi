#include "ldr_task.h"

static const char *TAG = "LDR";
static const int BUFFER_SIZE = 32;
static const float GAMMA = 0.7;
static const float RL10 = 50;

static uint32_t luminosity = 0;

void ldr_task(void *arg) 
{
    local_data_t ldr_data;
    ldr_data.type = LDR;
    while (1) {
        // Read the LDR value from ADC
        int val = 0;
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, adc_chan, &val));
        float voltage = val / 4096.0 * 5;
        float resistance = 2000 * voltage / (1 - voltage / 5);
        luminosity = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA)) + 0.49999;

        // Send data to the queue
        ldr_data.data.ldr_data.luminosity = luminosity;
        if (ldr_queue != NULL && xQueueSend(ldr_queue, &ldr_data, 0) != pdPASS) {
            ESP_LOGE(TAG, "Failed to send LDR data to queue");
        }
        
        // ESP_LOGI(TAG, "Luminosity: %d, is_dark: %d", ldr_data.luminosity, ldr_data.is_dark);
        // char json[BUFFER_SIZE];
        // snprintf(json, BUFFER_SIZE, "{\"lumi\": %d, \"is_dark\": %d}", ldr_data.luminosity, ldr_data.is_dark);
        // send_telemetry(json);
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