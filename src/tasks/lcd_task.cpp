#include "lcd_task.h"

static const char *TAG = "LCD";
static char data[LCD_BUFFER_SIZE]; 

DisplayILI9341_240x320x16_SPI display(LCD_RST_PIN, {LCD_SPI_HOST, LCD_CS_PIN, LCD_DC_PIN, LCD_CLOCK_SPEED_HZ, LCD_SCK_PIN, LCD_MOSI_PIN});

static void lcd_display_task(void *args)
{
    while (1) {
        if (strcmp(data, "") != 0) {
            display.printFixed(0, 0, "");
            vTaskDelay(pdMS_TO_TICKS(LCD_FLUSH_TIMER));
            display.printFixed(0, 0, data);
            strcpy(data, "");
        }
        vTaskDelay(pdMS_TO_TICKS(LCD_DISPLAY_TIMER));
    }
}

static void lcd_fetch_data_task(void *args) {
    while (1) {
        snprintf(data, 
                 LCD_BUFFER_SIZE, 
                "Temperature %.2fC\nHumidity %.2f%%\nLuminousity %lu\nDate: %2d/%2d/%2d\nTime: %2d/%2d\n", 
                 data_storage.get_dht22_data().dht22_data.temperature, 
                 data_storage.get_dht22_data().dht22_data.humidity,
                 data_storage.get_ldr_data().ldr_data.luminosity,
                 data_storage.get_ntp_data().ntp_data.day,
                 data_storage.get_ntp_data().ntp_data.month,
                 data_storage.get_ntp_data().ntp_data.year,
                 data_storage.get_ntp_data().ntp_data.hour,
                 data_storage.get_ntp_data().ntp_data.minute
                );
        ESP_LOGI(TAG, "Data fetched: \n%s", data);
        vTaskDelay(pdMS_TO_TICKS(LCD_FETCH_TIMER));
    }
}

void lcd_task_init() {
    display.begin();
    display.setFixedFont(ssd1306xled_font8x16);
    
    BaseType_t fetch_data_task_created = xTaskCreate(lcd_fetch_data_task, "lcd_fetch_data_task", 4096, NULL, 1, NULL);
    if (fetch_data_task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create lcd_fetch_data_task");
        return;
    }
    ESP_LOGI(TAG, "lcd_fetch_data_task initialized");

    BaseType_t display_task_created = xTaskCreate(lcd_display_task, "lcd_display_task", 2048, NULL, 2, NULL);
    if (display_task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create lcd_display_task");
        return;
    }
    ESP_LOGI(TAG, "lcd_display_task initialized");
}