#include "ili9341_task.h"

void send_byte() {
    gpio_set_level(CS_PIN, 0);
    
}

void ili9341_task_init(void) {
    
}