#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

#include <global.h>

extern adc_oneshot_unit_handle_t adc1_handle;
extern adc_channel_t adc_chan;

extern esp_lcd_panel_io_handle_t io_handle;
extern esp_lcd_panel_handle_t panel_handle;

void hardware_config_init(void);

#endif // HARDWARE_CONFIG_H_