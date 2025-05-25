#include "data_config.h"

DataStorage::DataStorage() {
    dht22_data = {
        .temperature = 0,
        .humidity = 0,
    };

    ldr_data = {
        .luminosity = 0,
    };

    ntp_data = {
        .minute = 0,
        .hour = 0,
        .day = 0,
        .month = 0,
        .year = 0,
    };

    data_semaphore = xSemaphoreCreateMutex();
    if (data_semaphore == NULL) {
        ESP_LOGE(TAG, "Failed to create semaphore");
    }
}

DataStorage::~DataStorage() {
    if (data_semaphore != NULL) {
        vSemaphoreDelete(data_semaphore);
    }
}

void DataStorage::set_dht22_data(const dht22_data_t &data) {
    set_dht22_data(data.temperature, data.humidity);
}

void DataStorage::set_dht22_data(const float &temperature, const float &humidity) {
    if (xSemaphoreTake(data_semaphore, pdMS_TO_TICKS(DATA_SEMAPHORE_TIMER)) == pdTRUE) {
        dht22_data.humidity = humidity;
        dht22_data.temperature = temperature;
        xSemaphoreGive(data_semaphore);
    }
    else {
        ESP_LOGE(TAG, "Failed to set DHT22 data");
    }
}

void DataStorage::set_ldr_data(const ldr_data_t &data) {
    set_ldr_data(data.luminosity);
}

void DataStorage::set_ldr_data(const uint32_t &luminosity) {
    if (xSemaphoreTake(data_semaphore, pdMS_TO_TICKS(DATA_SEMAPHORE_TIMER)) == pdTRUE) {
        ldr_data.luminosity = luminosity;
        xSemaphoreGive(data_semaphore);
    }
    else {
        ESP_LOGE(TAG, "Failed to set LDR data");
    }
}

void DataStorage::set_ntp_data(const ntp_data_t &data) {
    set_ntp_data(data.minute, data.hour, data.day, data.month, data.year);
}

void DataStorage::set_ntp_data(const uint8_t &minute, const uint8_t &hour, const uint8_t &day, const uint8_t &month, const uint16_t &year) {
    if (xSemaphoreTake(data_semaphore, pdMS_TO_TICKS(DATA_SEMAPHORE_TIMER)) == pdTRUE) {
        ntp_data.minute = minute;
        ntp_data.hour = hour;
        ntp_data.day = day;
        ntp_data.month = month;
        ntp_data.year = year;
        xSemaphoreGive(data_semaphore);
    }
    else {
        ESP_LOGE(TAG, "Failed to set NTP data");
    }
}

void DataStorage::set_hc_sr04_data(const hc_sr04_data_t &data) {
    set_hc_sr04_data(data.distance);
}

void DataStorage::set_hc_sr04_data(const uint16_t &distance) {
    if (xSemaphoreTake(data_semaphore, pdMS_TO_TICKS(DATA_SEMAPHORE_TIMER)) == pdTRUE) {
        hc_sr04_data.distance = distance;
        xSemaphoreGive(data_semaphore);
    }
    else {
        ESP_LOGE(TAG, "Failed to set LDR data");
    }
}

dht22_data_t DataStorage::get_dht22_data() {
    dht22_data_t data = {0, 0};
    if (xSemaphoreTake(data_semaphore, pdMS_TO_TICKS(DATA_SEMAPHORE_TIMER)) == pdTRUE) {
        data = dht22_data;
        xSemaphoreGive(data_semaphore);
    }
    else {
        ESP_LOGE(TAG, "Failed to get DHT22 data");
    }
    return data;
}

ldr_data_t DataStorage::get_ldr_data() {
    ldr_data_t data = {0};
    if (xSemaphoreTake(data_semaphore, pdMS_TO_TICKS(DATA_SEMAPHORE_TIMER)) == pdTRUE) {
        data = ldr_data;
        xSemaphoreGive(data_semaphore);
    }
    else {
        ESP_LOGE(TAG, "Failed to get LDR data");
    }
    return data;
}

ntp_data_t DataStorage::get_ntp_data() {
    ntp_data_t data = {0, 0, 0, 0, 0};
    if (xSemaphoreTake(data_semaphore, pdMS_TO_TICKS(DATA_SEMAPHORE_TIMER)) == pdTRUE) {
        data = ntp_data;
        xSemaphoreGive(data_semaphore);
    }
    else {
        ESP_LOGE(TAG, "Failed to get NTP data");
    }
    return data;
}

hc_sr04_data_t DataStorage::get_hc_sr04_data() {
    hc_sr04_data_t data = {0};
    if (xSemaphoreTake(data_semaphore, pdMS_TO_TICKS(DATA_SEMAPHORE_TIMER)) == pdTRUE) {
        data = hc_sr04_data;
        xSemaphoreGive(data_semaphore);
    }
    else {
        ESP_LOGE(TAG, "Failed to get NTP data");
    }
    return data;
}

DataStorage data_storage;

// DisplayItem Section
static display_value_t temp_value, humid_value, lum_value, date_value, time_value;

static display_value_t* get_temp() {
    temp_value.f_value = data_storage.get_dht22_data().temperature;
    return &temp_value;
}

static display_value_t* get_humidity() {
    humid_value.f_value = data_storage.get_dht22_data().humidity;
    return &humid_value;
}

static display_value_t* get_luminosity() {
    lum_value.u_value = data_storage.get_ldr_data().luminosity;
    return &lum_value;
}

static display_value_t* get_date() {
    date_value.date.day = data_storage.get_ntp_data().day;
    date_value.date.month = data_storage.get_ntp_data().month;
    date_value.date.year = data_storage.get_ntp_data().year;
    return &date_value;
}

static display_value_t* get_time() {
    time_value.time.hour = data_storage.get_ntp_data().hour;
    time_value.time.minute = data_storage.get_ntp_data().minute;
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