#ifndef WIFI_TASK_H_
#define WIFI_TASK_H_

#include <global.h>

extern volatile bool wifi_is_connected;
void wifi_task_init();

#endif // WIFI_TASK_H_