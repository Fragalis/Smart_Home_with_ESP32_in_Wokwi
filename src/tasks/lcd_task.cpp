#include "lcd_task.h"
#include "lcdgfx.h"
#include "lcdgfx_gui.h"

DisplayILI9341_240x320x16_SPI display(LCD_RST_PIN, {LCD_SPI_HOST, LCD_CS_PIN, LCD_DC_PIN, LCD_CLOCK_SPEED_HZ, LCD_SCK_PIN, LCD_MOSI_PIN});

static void textDemo()
{
    display.setFixedFont(ssd1306xled_font6x8);
    display.clear();
    display.setColor(RGB_COLOR16(255,255,0));
    display.printFixed(0,  8, "Normal text", STYLE_NORMAL);
    display.setColor(RGB_COLOR16(0,255,0));
    display.printFixed(0, 16, "bold text?", STYLE_BOLD);
    display.setColor(RGB_COLOR16(0,255,255));
    display.printFixed(0, 24, "Italic text?", STYLE_ITALIC);
    display.setColor(RGB_COLOR16(255,255,255));
    display.invertColors();
    display.printFixed(0, 32, "Inverted bold?", STYLE_BOLD);
    display.invertColors();
    lcd_delay(3000);
}

void lcd_task_init() {
    display.begin();
    display.setFixedFont(ssd1306xled_font6x8);

    while (1) {
        textDemo();
        display.fill( 0x00 );
        display.setColor(RGB_COLOR16(255,255,255));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}