#ifndef DATA_CONFIG_H_
#define DATA_CONFIG_H_

#include <global.h>

typedef struct {
    float temperature;
    float humidity;
} dht22_data_t;

typedef struct {
    int32_t luminosity;
} ldr_data_t;

typedef struct {
    int8_t minute;
    int8_t hour;
    int8_t day;
    int8_t month;
    int16_t year;
} ntp_data_t;

typedef struct {
    int16_t distance;
} hc_sr04_data_t;

class DataStorage {
private:
    const char *TAG = "DATA";

    dht22_data_t dht22_data;
    ldr_data_t ldr_data;
    ntp_data_t ntp_data;
    hc_sr04_data_t hc_sr04_data;

    SemaphoreHandle_t data_semaphore;
public:
    DataStorage();
    ~DataStorage();

    void set_dht22_data(const dht22_data_t &data);
    void set_dht22_data(const float &temperature, const float &humidity);

    void set_ldr_data(const ldr_data_t &data);
    void set_ldr_data(const int32_t &luminosity);

    void set_ntp_data(const ntp_data_t &data);
    void set_ntp_data(const int8_t &minute, const int8_t &hour, const int8_t &day, const int8_t &month, const int16_t &year);
    
    void set_hc_sr04_data(const hc_sr04_data_t &data);
    void set_hc_sr04_data(const int16_t &distance);

    dht22_data_t get_dht22_data();
    ldr_data_t get_ldr_data();
    ntp_data_t get_ntp_data();
    hc_sr04_data_t get_hc_sr04_data();
};

typedef union {
    float f_value;    // For temperature, humidity
    uint32_t u_value; // For luminosity
    struct {
        int day, month, year; // For date
    } date;
    struct {
        int hour, minute; // For time
    } time;
} display_value_t;

enum display_type { DHT22, LDR, DATE, TIME } ; // data types
typedef struct {
    const char* label;                      // e.g., "Temp: "
    display_value_t* (*get_value)();        // Function to fetch the value from DataStorage
    const char* format;                     // e.g., "%.2fC\n"
    display_type type;
} display_item_t;

extern display_item_t display_items[];
extern DataStorage data_storage;
extern const int num_items;

#endif // DATA_AGGREGATION_CONFIG_H_