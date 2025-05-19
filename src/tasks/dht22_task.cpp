#include "dht22_task.h"

// Datasheet https://cdn.sparkfun.com/assets/f/7/d/9/c/DHT22.pdf
static const char *TAG = "DHT22";

static float temperature = 0.0;
static float humidity = 0.0;

static bool await_signal(uint8_t uInterval, int signal) 
{
    uint8_t uSeconds = 0;
    while (gpio_get_level(DHT_PIN) != signal) {
        if (uSeconds > uInterval) {
            return false;
        }
        uSeconds++;
        esp_rom_delay_us(1);
    }
    return true;
}

// Send a signal to the DHT22 sensor and await response
static esp_err_t send_signal() 
{
    // Set the pin to output mode to send a signal
    // MCU sends a low signal for 2ms and a high signal for 40us
    gpio_set_direction(DHT_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT_PIN, 0); // Pull the pin low
    vTaskDelay(10 / portTICK_PERIOD_MS); // Wait for 10ms
    gpio_set_level(DHT_PIN, 1); // Release the pin
    esp_rom_delay_us(40); // Wait for 40us

    // Set the pin to input mode to read the response
    gpio_set_direction(DHT_PIN, GPIO_MODE_INPUT);
    gpio_get_level(DHT_PIN); // Read the pin level

    // Sensor sends a low signal for 80us
    // Await the next high signal from sensor
    if (!await_signal(100, 1)) {
        ESP_LOGE(TAG, "Unable to receive data from sensor");
        return ESP_FAIL;
    }

    // Sensor sends a higher signal for 80us
    // Await the next low signal from sensor
    if (!await_signal(100, 0)) {
        ESP_LOGE(TAG, "Request timeout");
        return ESP_FAIL;
    }
    
    // Sensor is ready to send data
    return ESP_OK;
}

// Read data from the DHT22 sensor
static esp_err_t read_data(float *temp, float *hum) 
{
    uint8_t data[5] = {0};
    for (uint8_t i = 0; i < 40; ++i) {
        // Sensor send 1 bit of low signal for 50us
        // Await the next high signal from sensor
        if (!await_signal(100, 1)) {
            ESP_LOGE(TAG, "Request timeout");
            return ESP_FAIL;
        }
        esp_rom_delay_us(50); // Wait for 50us
        data[i/8] <<= 1; // Shift the data to the left
        if (gpio_get_level(DHT_PIN) > 0) data[i/8] |= 1; // Set the last bit to 1 if sensor signal is high

        // Await the next low signal from sensor
        if (!await_signal(100, 0)) {
            ESP_LOGE(TAG, "Request timeout");
            return ESP_FAIL;
        }
    }

    // Checksum
    if (((data[0] + data[1] + data[2] + data[3]) & 0xFF) != data[4]) {
        ESP_LOGE(TAG, "Checksum error");
        return ESP_FAIL;
    }

    // Convert the data to temperature and humidity
    *hum = ((data[0] << 8) + data[1]) / 10.00;
    *temp = (((data[2] & 0x7F) << 8) + data[3]) / 10.00;
    if (data[2] & 0x80) *temp = -(*temp); // Negative temperature
    return ESP_OK;
}

void dht22_task(void *arg) 
{
    while (1) {
        // Send signal to the DHT22 sensor
        esp_err_t err = send_signal();
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send signal");
            vTaskDelay(pdMS_TO_TICKS(DHT22_SEND_TIMER));
            continue;
        }
        // Read data from the DHT22 sensor
        err = read_data(&temperature, &humidity);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to read data");
            vTaskDelay(pdMS_TO_TICKS(DHT22_READ_TIMER));
            continue;
        }
        // Log the temperature and humidity
        ESP_LOGI(TAG, "Temperature: %.2f °C, Humidity: %.2f %%", temperature, humidity);

        char json[64];
        snprintf(json, 64, "{\"temperature\": %.2f, \"humidity\": %.2f}", temperature, humidity);
        send_telemetry(json);

        // Delay for 5 seconds before the next reading
        vTaskDelay(pdMS_TO_TICKS(DHT22_DELAY_TIMER));
    }
}

void dht22_task_init() {
    // Create the DHT22 task
    BaseType_t task_created = xTaskCreate(dht22_task, "dht22_task", 4096, NULL, 1, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create dht22_task");
        return;
    }
    ESP_LOGI(TAG, "dht22_task initialized");
}