#include "lcd_task.h"

static const char *TAG = "LCD";
static char data[LCD_BUFFER_SIZE]; 

DisplayILI9341_240x320x16_SPI display(LCD_RST_PIN, {LCD_SPI_HOST, LCD_CS_PIN, LCD_DC_PIN, LCD_CLOCK_SPEED_HZ, LCD_SCK_PIN, LCD_MOSI_PIN});

static void lcd_display_task(void *args)
{
    while (1) {
        if (strcmp(data, "") != 0) {
            display.clear();
            vTaskDelay(pdMS_TO_TICKS(LCD_FLUSH_TIMER));
            display.printFixed(0, 0, data);
            strcpy(data, "");
        }
        vTaskDelay(pdMS_TO_TICKS(LCD_DISPLAY_TIMER));
    }
}

static void lcd_fetch_data_task(void *args) {
    while (1) {
        char temp_buffer[32];
        for (int i = 0; i < num_items; i++) {
            display_value_t* value = display_items[i].get_value();
            switch (display_items[i].type) {
                case DHT22:
                    snprintf(temp_buffer, sizeof(temp_buffer), display_items[i].format, value->f_value);
                    break;
                case LDR:
                    snprintf(temp_buffer, sizeof(temp_buffer), display_items[i].format, value->u_value);
                    break;
                case DATE:
                    snprintf(temp_buffer, sizeof(temp_buffer), display_items[i].format, 
                             value->date.day, value->date.month, value->date.year);
                    break;
                case TIME:
                    snprintf(temp_buffer, sizeof(temp_buffer), display_items[i].format, 
                             value->time.hour, value->time.minute);
                    break;
                default:
                    ESP_LOGW(TAG, "label %s with format %s", display_items[i].label, display_items[i].format);
            }
            strncat(data, display_items[i].label, LCD_BUFFER_SIZE - strlen(data) - 1);
            strncat(data, temp_buffer, LCD_BUFFER_SIZE - strlen(data) - 1);
        }
        ESP_LOGI(TAG, "Data fetched");
        // ESP_LOGI(TAG, "%s", data);
        vTaskDelay(pdMS_TO_TICKS(LCD_FETCH_TIMER));
    }
}

void lcd_task_init() {
    display.begin();
    display.setFixedFont(ssd1306xled_font8x16);
    gpio_set_level(LCD_LED_PIN, 1);

    BaseType_t fetch_data_task_created = xTaskCreate(lcd_fetch_data_task, "lcd_fetch_data_task", 4096, NULL, 2, NULL);
    if (fetch_data_task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create lcd_fetch_data_task");
        return;
    }
    ESP_LOGI(TAG, "lcd_fetch_data_task initialized");

    BaseType_t display_task_created = xTaskCreate(lcd_display_task, "lcd_display_task", 2048, NULL, 3, NULL);
    if (display_task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create lcd_display_task");
        return;
    }
    ESP_LOGI(TAG, "lcd_display_task initialized");
}