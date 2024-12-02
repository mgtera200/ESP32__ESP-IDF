# ESP32 Wi-Fi Dual Mode (STA + AP) Example

This program demonstrates how to configure the ESP32 to operate in both **Station (STA)** and **Access Point (AP)** modes simultaneously. The ESP32 connects to a Wi-Fi network in STA mode, and upon successful connection, it starts an Access Point (AP) with a configurable SSID and password. The program logs the Wi-Fi connection status, Access Point setup, and handles Wi-Fi events such as connection, disconnection, and IP address assignment.

## Author

- **Mostafa Gamal Tera**
- **Date**: December 2024

## Dependencies

- ESP32 platform with FreeRTOS
- ESP-IDF libraries:
  - `esp_wifi`
  - `esp_netif`
  - `nvs_flash`
  - `esp_event`
  - `esp_log`

## Usage

1. **Initialize NVS** (Non-Volatile Storage) for Wi-Fi and networking.
2. Set up Wi-Fi in **station mode** (STA) to connect to a specified network.
3. On successful connection, configure and start the **Access Point (AP)** mode.
4. Handle Wi-Fi events (connection, disconnection, IP obtained) and switch between modes.

## Functions

- `wifi_event_handler()`:
  - Handles Wi-Fi events such as connection, disconnection, and IP address obtained.
- `wifi_init_sta()`:
  - Initializes Wi-Fi in station mode and connects to the network.
- `wifi_init_ap()`:
  - Initializes Wi-Fi in Access Point mode after connecting to the network.
- `app_main()`:
  - The main function where initialization and event handling occur.

## Wi-Fi Configuration

### Station Mode (STA):
- **SSID**: `TERA`
- **Password**: `teratera1`

### Access Point Mode (AP):
- **SSID**: `ESP32-AP`
- **Password**: `teratera1`
- **Wi-Fi Channel**: `1`

## How It Works

1. The program starts by initializing **Non-Volatile Storage (NVS)** to store Wi-Fi settings.
2. The **Station mode** (`wifi_init_sta`) is initialized to connect to the specified Wi-Fi network (`WIFI_SSID` and `WIFI_PASS`).
3. Once connected, the **Access Point mode** (`wifi_init_ap`) is started with the configured SSID and password.
4. The program handles Wi-Fi events such as:
   - **WIFI_EVENT_STA_START**: Attempts to connect to the network.
   - **WIFI_EVENT_STA_CONNECTED**: Starts the Access Point mode after successful connection.
   - **WIFI_EVENT_STA_DISCONNECTED**: Attempts to reconnect to the network if disconnected.
   - **IP_EVENT_STA_GOT_IP**: Logs the IP address after successful connection.

## Example Log Output

```bash
I (2751) WiFi_Example: Wi-Fi started. Connecting...
I (2761) WiFi_Example: Wi-Fi connected
I (2761) WiFi_Example: Starting Access Point...
I (2761) WiFi_Example: Access Point started. SSID: ESP32-AP, Password: teratera1