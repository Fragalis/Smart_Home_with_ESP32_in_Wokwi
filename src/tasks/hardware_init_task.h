#ifndef HARDWARE_INIT_TASK_H_
#define HARDWARE_INIT_TASK_H_

#include <global.h>

extern adc_oneshot_unit_handle_t adc1_handle;
extern adc_channel_t adc_chan;

void hardware_init(void);

#endif // HARDWARE_INIT_TASK_H_