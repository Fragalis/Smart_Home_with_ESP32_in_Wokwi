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
            uint32_t luminosity;
        } ldr_data;
        struct {
            uint8_t minute;
            uint8_t hour;
            uint8_t day;
            uint8_t month;
            uint8_t year;
        } ntp_data;
    } data;
} local_data_t;

extern QueueHandle_t dht22_queue;
extern QueueHandle_t ldr_queue;
extern QueueHandle_t ntp_queue;

void data_config_init(void);

#endif // DATA_AGGREGATION_CONFIG_H_