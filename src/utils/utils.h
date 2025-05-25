#ifndef UTILS_H_
#define UTILS_H_

#include "global.h"

uint16_t await_signal(gpio_num_t pin, uint16_t timeout, int signal);

#endif // UTILS_H_