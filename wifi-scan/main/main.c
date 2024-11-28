#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

// Function to scan for available Wi-Fi networks
void scann() {
    // Configure Wi-Fi scan parameters
    wifi_scan_config_t scan_config = {
        .ssid = NULL,               // NULL indicates scan for all SSIDs
        .bssid = NULL,              // NULL means scan for all BSSIDs
        .channel = 0,               // 0 means scan all channels
        .show_hidden = true         // Show hidden networks (those not broadcasting SSID)
    };

    printf("Start scanning...\n");

    // Start scanning for networks in blocking mode
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

    // Notify when scan is completed
    printf("Scan completed!\n");

    // Declare variables to store the number of APs found and their records
    uint16_t ap_num = 0;  // Store number of access points found
    wifi_ap_record_t ap_records[20];  // Store the details of found APs

    // Retrieve the number of APs found in the last scan
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_num));

    // Retrieve the list of AP records (SSID, RSSI, MAC, etc.)
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_num, ap_records));

    // Output the number of found APs
    printf("Found %d access points:\n", ap_num);
    printf("               SSID              | Channel | RSSI |   MAC\n\n");

    // Loop through and print details for each AP found
    for (int i = 0; i < ap_num; i++) {
        printf("%32s | %7d | %4d | %02x:%02x:%02x:%02x:%02x:%02x\n",
            ap_records[i].ssid, ap_records[i].primary, ap_records[i].rssi,
            ap_records[i].bssid[0], ap_records[i].bssid[1], ap_records[i].bssid[2],
            ap_records[i].bssid[3], ap_records[i].bssid[4], ap_records[i].bssid[5]);
    }
}

// Main application entry point
void app_main() {
    // Initialize NVS (Non-Volatile Storage) for Wi-Fi settings
    ESP_ERROR_CHECK(nvs_flash_init());

    // Initialize ESP-NETIF library to handle networking interfaces
    ESP_ERROR_CHECK(esp_netif_init());

    // Initialize the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize default Wi-Fi configuration for station mode
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));

    // Set the Wi-Fi mode to Station (STA), i.e., client mode
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Start the Wi-Fi driver in Station mode
    ESP_ERROR_CHECK(esp_wifi_start());

    // Continuously scan for available Wi-Fi networks in a loop
    while (1) {
        scann();  // Call the scanning function
        vTaskDelay(3000 / portTICK_PERIOD_MS);  // Wait for 3 seconds before next scan
    }
}
