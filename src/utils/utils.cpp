#include "utils.h"

uint16_t await_signal(gpio_num_t pin, uint16_t timeout, int signal) {
    uint16_t uSeconds = 1;
    while (gpio_get_level(pin) != signal) {
        if (uSeconds > timeout) {
            return 0;
        }
        esp_rom_delay_us(1);
        uSeconds++;
    }
    return uSeconds;
}