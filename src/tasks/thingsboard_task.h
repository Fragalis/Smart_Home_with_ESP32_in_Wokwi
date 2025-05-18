#ifndef THINGSBOARD_TASK_H_
#define THINGSBOARD_TASK_H_

#include <global.h>

void send_telemetry(const char *json);
void thingsboard_task_init();

#endif // THINGSBOARD_TASK_H_