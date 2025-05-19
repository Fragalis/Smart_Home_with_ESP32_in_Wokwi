#ifndef DATA_CONFIG_H_
#define DATA_CONFIG_H_

#include <global.h>

enum data_type { DHT22, LDR, NTP };

typedef struct {
    data_type type;
    union {
        struct {
            float temperature;
            float humidity;
        } dht22_data;
        struct {
            int luminosity; // ADC value (0–4095)
            int is_dark;    // DO state (1 for dark)
        } ldr_data;
        struct {
            int hour;
        } ntp_data;
    } data;
} local_data_t;

extern QueueHandle_t dht22_queue;
extern QueueHandle_t ldr_queue;
extern QueueHandle_t ntp_queue;

void data_config_init(void);

#endif // DATA_AGGREGATION_CONFIG_H_