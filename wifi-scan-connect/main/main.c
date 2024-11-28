#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"

// Replace with your network credentials
#define WIFI_SSID "TERA"      // Wi-Fi SSID (network name)
#define WIFI_PASS "teratera1" // Wi-Fi password

static const char *TAG = "WiFi_Example"; // Tag used in logging for easier identification

// Event handler for Wi-Fi events
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    // Handle Wi-Fi events based on their event_base and event_id
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        // Wi-Fi started, attempt to connect to the network
        ESP_LOGI(TAG, "Wi-Fi started. Connecting...");
        esp_wifi_connect();  // Start the connection process
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        // Wi-Fi disconnected, attempt to reconnect
        ESP_LOGI(TAG, "Disconnected from Wi-Fi. Reconnecting...");
        esp_wifi_connect();  // Start the reconnection process
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        // IP address obtained, connection is successful
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Connected! IP Address: " IPSTR, IP2STR(&event->ip_info.ip));  // Log the assigned IP address
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        // Wi-Fi connected, waiting for IP address
        ESP_LOGI(TAG, "Wi-Fi connected, waiting for IP...");
    }
}


// Function to initialize Wi-Fi and connect to the network
void wifi_init_sta() {
    // Initialize Wi-Fi driver with default configuration
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));  // Check for initialization errors

    // Register Wi-Fi event handlers for various events (e.g., connection, disconnection, IP received)
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

    // Configure Wi-Fi in station mode (client mode)
    wifi_config_t sta_config = {
        .sta = {
            .ssid = WIFI_SSID,           // SSID of the Wi-Fi network
            .password = WIFI_PASS,       // Password for the Wi-Fi network
            .threshold.authmode = WIFI_AUTH_WPA2_PSK, // Set WPA2-PSK security mode
        },
    };

    // Set Wi-Fi mode to STA (Station mode) to act as a client
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Apply the Wi-Fi configuration
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));

    // Start the Wi-Fi driver to begin the connection process
    ESP_ERROR_CHECK(esp_wifi_start());
}

// Main application entry point
void app_main() {
    // Initialize NVS (Non-Volatile Storage), required for Wi-Fi functionality
    ESP_ERROR_CHECK(nvs_flash_init());

    // Initialize the ESP-NETIF library, required for managing network interfaces
    ESP_ERROR_CHECK(esp_netif_init());

    // Initialize the default event loop (needed for event handling)
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize and configure Wi-Fi as a station (client)
    wifi_init_sta();

    // Keep the main task running; the Wi-Fi connection process is handled by events
    while (1) {
        vTaskDelay(10000 / portTICK_PERIOD_MS); // Task delay to prevent the app from exiting
    }
}
