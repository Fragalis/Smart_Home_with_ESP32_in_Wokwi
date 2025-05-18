#include "ldr_task.h"
#include "esp_adc/adc_oneshot.h"
#include <math.h>

// LDR pin
#define LDR_DO_PIN GPIO_NUM_21
#define LDR_AO_PIN GPIO_NUM_10

const char *TAG = "LDR";
const float GAMMA = 0.7;
const float RL10 = 50;

// Structure for LDR data
typedef struct {
    int luminosity; // ADC value (0–4095)
    int is_dark;    // DO state (1 for dark)
} ldr_data_t;

void ldr_task(void *arg)
{
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t adc1_init_config = {
        .unit_id = ADC_UNIT_1,
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc1_init_config, &adc1_handle));
    adc_oneshot_chan_cfg_t adc1_config = {
        .atten = ADC_ATTEN_DB_0,
        .bitwidth = ADC_BITWIDTH_12,
    };
    adc_channel_t adc_chan;
    ESP_ERROR_CHECK(adc_oneshot_io_to_channel(LDR_AO_PIN, &adc1_init_config.unit_id, &adc_chan));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, adc_chan, &adc1_config));

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LDR_DO_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

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
        vTaskDelay(5000 / portTICK_PERIOD_MS);
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