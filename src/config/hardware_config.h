#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

#include <global.h>

extern adc_oneshot_unit_handle_t adc1_handle;
extern adc_channel_t adc_chan;

extern spi_device_handle_t spi;

void hardware_config_init(void);

#endif // HARDWARE_CONFIG_H_