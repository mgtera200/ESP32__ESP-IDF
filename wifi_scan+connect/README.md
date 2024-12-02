# Wi-Fi Connection Example for ESP32 with FreeRTOS

## Overview

This program demonstrates how to connect the ESP32 to a Wi-Fi network using FreeRTOS and the ESP-IDF Wi-Fi API. It handles various Wi-Fi events (such as starting the connection, disconnecting, and obtaining an IP address) and logs the connection status and IP address when successfully connected.

### Author
- Mostafa Gamal Tera  
- Date: December 2024

## Dependencies

- **ESP32 platform with FreeRTOS**
- **ESP-IDF libraries**: `esp_wifi`, `esp_netif`, `nvs_flash`, `esp_event`, `esp_log`

## Features

- Connect to a specified Wi-Fi network.
- Handle Wi-Fi events such as connecting, disconnecting, and IP address assignment.
- Log the connection status and obtained IP address to the console.

## Usage

1. Initialize NVS (Non-Volatile Storage) for Wi-Fi and networking.
2. Set up Wi-Fi in station mode and attempt to connect to the specified network.
3. Handle Wi-Fi connection events and log the connection status and IP address when successfully connected.

### How to Configure

- Replace the `WIFI_SSID` and `WIFI_PASS` macros with your network's SSID and password.

```c
#define WIFI_SSID "YourNetworkSSID"
#define WIFI_PASS "YourNetworkPassword"