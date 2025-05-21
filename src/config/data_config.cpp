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