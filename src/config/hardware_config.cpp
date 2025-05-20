#include "hardware_config.h"

const char *TAG = "HARDWARE";

adc_oneshot_unit_handle_t adc1_handle = NULL;
adc_channel_t adc_chan = ADC_CHANNEL_0;

spi_device_handle_t spi = NULL;


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

static void init_lcd(void) 
{
    spi_bus_config_t spi_conf = {
        .mosi_io_num = LCD_MOSI_PIN,
        .miso_io_num = LCD_MISO_PIN,
        .sclk_io_num = LCD_SCK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_DRAW_BUFFER_SIZE * 320 * 2 + 8,
    };
    spi_bus_initialize(LCD_SPI_HOST, &spi_conf, SPI_DMA_CH_AUTO);

    spi_device_interface_config_t devcfg = {
        .mode = 0,                              //SPI mode 0
        .clock_speed_hz = LCD_CLOCK_SPEED_HZ,   //Clock out at 10 MHz
        .spics_io_num = LCD_CS_PIN,             //CS pin
        .queue_size = 7,                        //We want to be able to queue 7 transactions at a time
        .pre_cb = lcd_spi_pre_transfer_callback, //Specify pre-transfer callback to handle D/C line
    };   
    ESP_ERROR_CHECK(spi_bus_add_device(LCD_SPI_HOST, &devcfg, &spi));
}

static void init_gpio_pins(void) 
{
    // Initialize GPIO configuration
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_PIN)
                      | (1ULL << STEPPER_DIRECTION_PIN)
                      | (1ULL << STEPPER_STEP_PIN)
                      | (1ULL << LIGHT_PIN)
                      | (1ULL << LCD_DC_PIN) 
                      | (1ULL << LCD_RST_PIN) 
                      | (1ULL << LCD_LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    // Configure GPIO pins
    ESP_ERROR_CHECK(gpio_config(&io_conf));
}

void hardware_config_init(void)
{
    init_gpio_pins();
    init_adc();
    init_lcd();
    init_nvs_flash();
}