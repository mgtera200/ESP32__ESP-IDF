# Wi-Fi Network Scanner for ESP32 with FreeRTOS

## Overview

This program demonstrates how to scan for available Wi-Fi networks using the ESP32 and FreeRTOS. The program continuously scans for networks, retrieves their details (SSID, RSSI, Channel, MAC address), and prints the information to the console.

### Author
- Mostafa Gamal Tera  
- Date: December 2024

## Dependencies

- **ESP32 platform with FreeRTOS**
- **ESP-IDF libraries**: `esp_wifi`, `esp_netif`, `nvs_flash`

## Features

- Scan for available Wi-Fi networks (SSIDs, RSSI, Channel, MAC address).
- Display network details on the console.
- Continuously scan at regular intervals.

## Usage

1. Initialize NVS (Non-Volatile Storage), Wi-Fi, and networking interfaces.
2. Configure and start Wi-Fi in Station mode.
3. Continuously scan for available Wi-Fi networks and display their information.

## Functions

- **scann()**: Scans for available Wi-Fi networks and prints their details.
- **app_main()**: Main function where Wi-Fi scanning is initialized and continuously run.

## How to Run

1. Make sure you have the ESP32 toolchain set up with the necessary ESP-IDF libraries.
2. Flash the firmware to your ESP32 device.
3. Upon booting, the program will continuously scan for available Wi-Fi networks and display their details.
