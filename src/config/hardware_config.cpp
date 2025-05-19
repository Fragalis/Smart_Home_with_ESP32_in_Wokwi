#include "hardware_config.h"

const char *TAG = "HARDWARE";

adc_oneshot_unit_handle_t adc1_handle = NULL;
adc_channel_t adc_chan = ADC_CHANNEL_0; // Adjust according to your setup

static void init_adc(void) 
{
    // Initialize ADC for LDR AO pin
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
    ESP_ERROR_CHECK(adc_oneshot_io_to_channel(LDR_AO_PIN, &adc1_init_config.unit_id, &adc_chan));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, adc_chan, &adc1_config));
    ESP_LOGI(TAG, "ADC initialized");
}

static void init_nvs_flash(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

static void init_gpio_pins(void) 
{
    // Initialize GPIO configuration
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_PIN)
                      | (1ULL << LDR_DO_PIN)
                      | (1ULL << STEPPER_DIRECTION_PIN)
                      | (1ULL << STEPPER_STEP_PIN)
                      | (1ULL << LIGHT_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    // Configure GPIO pins
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_LOGI(TAG, "GPIO pins initialized");
}

void hardware_config_init(void)
{
    init_gpio_pins();
    init_adc();
    init_nvs_flash();
}