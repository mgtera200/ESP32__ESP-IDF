#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"
#include <string.h>

// Replace with your network credentials
#define WIFI_SSID "TERA"  // Wi-Fi SSID for the station mode
#define WIFI_PASS "teratera1"  // Wi-Fi password for the station mode

// AP network details for Access Point mode
#define AP_SSID "ESP32-AP"  // SSID for the Access Point
#define AP_PASS "teratera1"  // Password for the Access Point
#define AP_CHANNEL 1  // Wi-Fi channel for the Access Point

static const char *TAG = "WiFi_Example";  // Log tag for identifying logs from this module

// Function prototypes
void wifi_init_ap();

// Event handler for Wi-Fi events
// Handles Wi-Fi connection, disconnection, and status changes
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        // Wi-Fi station mode started, attempt to connect to the configured Wi-Fi
        ESP_LOGI(TAG, "Wi-Fi started. Connecting...");
        esp_wifi_connect();  // Start the Wi-Fi connection
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        // Wi-Fi disconnected, attempt to reconnect
        ESP_LOGI(TAG, "Disconnected from Wi-Fi. Reconnecting...");
        esp_wifi_connect();  // Reconnect
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        // Wi-Fi connected, start the Access Point (AP) mode
        ESP_LOGI(TAG, "Wi-Fi connected");
        ESP_LOGI(TAG, "Starting Access Point...");
        wifi_init_ap();  // Initialize the AP mode after connecting to Wi-Fi
    }
}

// Function to initialize Wi-Fi in station mode (STA) and connect to the network
void wifi_init_sta() {
    // Initialize Wi-Fi with default configuration
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));  // Check for initialization errors

    // Register event handlers for Wi-Fi and IP events
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

    // Configure Wi-Fi in station mode (client)
    wifi_config_t sta_config = {
        .sta = {
            .ssid = WIFI_SSID,  // SSID of the Wi-Fi to connect to
            .password = WIFI_PASS,  // Password for the Wi-Fi
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,  // WPA2 security
        },
    };
    
    // Set the Wi-Fi mode to station (STA) and configure it
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));  // Set mode to STA
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));  // Set station configuration
    ESP_ERROR_CHECK(esp_wifi_start());  // Start the Wi-Fi driver
}

// Function to initialize Wi-Fi in Access Point mode (AP)
void wifi_init_ap() {
    // Configure Wi-Fi in Access Point mode
    wifi_config_t ap_config = {
        .ap = {
            .ssid = AP_SSID,  // SSID of the Access Point
            .ssid_len = strlen(AP_SSID),  // Length of SSID
            .password = AP_PASS,  // Password for the Access Point
            .max_connection = 4,  // Maximum number of connections to the AP
            .authmode = WIFI_AUTH_WPA2_PSK,  // WPA2 security
        },
    };

    // Set Wi-Fi mode to both Access Point and Station (AP+STA)
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));  // Set mode to AP+STA
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));  // Set AP configuration
    ESP_ERROR_CHECK(esp_wifi_start());  // Start the Wi-Fi driver in AP mode

    // Log the Access Point details for debugging
    ESP_LOGI(TAG, "Access Point started. SSID: %s, Password: %s", AP_SSID, AP_PASS);
}

// Main application entry point
void app_main() {
    // Initialize NVS (Non-Volatile Storage) to store Wi-Fi settings
    ESP_ERROR_CHECK(nvs_flash_init());  // Initialize NVS, used for storing Wi-Fi credentials

    // Initialize ESP-NETIF (network interface) for managing network configurations
    ESP_ERROR_CHECK(esp_netif_init());

    // Initialize the default event loop to handle Wi-Fi and IP events
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize Wi-Fi in station mode and attempt to connect to the network
    wifi_init_sta();

    // Optionally, continue to keep the main task running in a loop
    // This keeps the FreeRTOS task running, so other background tasks can be processed
    while (1) {
        vTaskDelay(10000 / portTICK_PERIOD_MS);  // Delay for 10 seconds before repeating
    }
}
