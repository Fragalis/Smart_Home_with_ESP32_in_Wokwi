#ifndef DATA_CONFIG_H_
#define DATA_CONFIG_H_

#include <global.h>

typedef union {
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
        uint16_t year;
    } ntp_data;
} local_data_t;

class DataStorage {
private:
    local_data_t dht22_data, ldr_data, ntp_data;
public:
    DataStorage();

    void set_dht22_data(const local_data_t &data);
    void set_dht22_data(const float &temperature, const float &humidity);

    void set_ldr_data(const local_data_t &data);
    void set_ldr_data(const uint32_t &luminosity);

    void set_ntp_data(const local_data_t &data);
    void set_ntp_data(const uint8_t &minute, const uint8_t &hour, const uint8_t &day, const uint8_t &month, const uint16_t &year);

    local_data_t get_dht22_data();
    local_data_t get_ldr_data();
    local_data_t get_ntp_data();
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
    const char* label;         // e.g., "Temp: "
    display_value_t* (*get_value)(); // Function to fetch the value from DataStorage
    const char* format;        // e.g., "%.2fC\n"
    display_type type;
} display_item_t;

extern display_item_t display_items[];
extern DataStorage data_storage;
extern const int num_items;

#endif // DATA_AGGREGATION_CONFIG_H_