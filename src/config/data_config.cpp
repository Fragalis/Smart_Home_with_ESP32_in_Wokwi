#include "data_config.h"

DataStorage::DataStorage() {
    dht22_data.dht22_data = {
        .temperature = 0,
        .humidity = 0,
    };

    ldr_data.ldr_data = {
        .luminosity = 0,
    };

    ntp_data.ntp_data = {
        .minute = 0,
        .hour = 0,
        .day = 0,
        .month = 0,
        .year = 0,
    };
}

void DataStorage::set_dht22_data(const local_data_t &data) {
    dht22_data.dht22_data.humidity = data.dht22_data.humidity;
    dht22_data.dht22_data.temperature = data.dht22_data.temperature;
}

void DataStorage::set_dht22_data(const float &temperature, const float &humidity) {
    dht22_data.dht22_data.humidity = humidity;
    dht22_data.dht22_data.temperature = temperature;
}

void DataStorage::set_ldr_data(const local_data_t &data) {
    ldr_data.ldr_data.luminosity = data.ldr_data.luminosity;
}

void DataStorage::set_ldr_data(const uint32_t &luminosity) {
    ldr_data.ldr_data.luminosity = luminosity;
}

void DataStorage::set_ntp_data(const local_data_t &data) {
    ntp_data.ntp_data.minute = data.ntp_data.minute;
    ntp_data.ntp_data.hour = data.ntp_data.hour;
    ntp_data.ntp_data.day = data.ntp_data.day;
    ntp_data.ntp_data.month = data.ntp_data.month;
    ntp_data.ntp_data.year = data.ntp_data.year;
}

void DataStorage::set_ntp_data(const uint8_t &minute, const uint8_t &hour, const uint8_t &day, const uint8_t &month, const uint16_t &year) {
    ntp_data.ntp_data.minute = minute;
    ntp_data.ntp_data.hour = hour;
    ntp_data.ntp_data.day = day;
    ntp_data.ntp_data.month = month;
    ntp_data.ntp_data.year = year;
}

local_data_t DataStorage::get_dht22_data() {
    return dht22_data;
}
local_data_t DataStorage::get_ldr_data() {
    return ldr_data;
}
local_data_t DataStorage::get_ntp_data() {
    return ntp_data;
}

DataStorage data_storage;

// DisplayItem Section
static display_value_t temp_value, humid_value, lum_value, date_value, time_value;

static display_value_t* get_temp() {
    temp_value.f_value = data_storage.get_dht22_data().dht22_data.temperature;
    return &temp_value;
}

static display_value_t* get_humidity() {
    humid_value.f_value = data_storage.get_dht22_data().dht22_data.humidity;
    return &humid_value;
}

static display_value_t* get_luminosity() {
    lum_value.u_value = data_storage.get_ldr_data().ldr_data.luminosity;
    return &lum_value;
}

static display_value_t* get_date() {
    date_value.date.day = data_storage.get_ntp_data().ntp_data.day;
    date_value.date.month = data_storage.get_ntp_data().ntp_data.month;
    date_value.date.year = data_storage.get_ntp_data().ntp_data.year;
    return &date_value;
}

static display_value_t* get_time() {
    time_value.time.hour = data_storage.get_ntp_data().ntp_data.hour;
    time_value.time.minute = data_storage.get_ntp_data().ntp_data.minute;
    return &time_value;
}

display_item_t display_items[] = {
    {"Temperature: ", get_temp, "%.2fC\n", DHT22},
    {"Humidity: ", get_humidity, "%.2f%%\n", DHT22},
    {"Lumimousity: ", get_luminosity, "%lu\n", LDR},
    {"Date: ", get_date, "%02d/%02d/%02d\n", DATE},
    {"Time: ", get_time, "%02d:%02d\n", TIME}
};

const int num_items = sizeof(display_items) / sizeof(display_items[0]);