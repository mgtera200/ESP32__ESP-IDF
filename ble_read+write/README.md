# BLE Server Example

**Author:** Mostafa Gamal Tera  
**Date:** December 2024  
**Project:** BLE Server Example  
**Description:** This project demonstrates a basic BLE server implementation using the NimBLE library on the ESP32 platform. It includes functionality for advertising, reading, and writing data over Bluetooth Low Energy (BLE), as well as utilizing FreeRTOS for task management.

## Features

- **BLE Advertising:** The server advertises its presence on the BLE network.
- **Read and Write Characteristics:** The server supports a GATT service with two characteristics:
  - **Read:** A characteristic that returns the data `0xDEAD` when read by the client.
  - **Write:** A characteristic that allows data to be written by the client, with the received data being printed to the console.
- **FreeRTOS Integration:** The server operates as a FreeRTOS task, utilizing the `nimble_port_freertos_init()` function to start the BLE stack and manage events.
- **GATT & GAP Services:** The code configures and uses GATT (Generic Attribute Profile) and GAP (Generic Access Profile) services to handle the BLE protocol.

## Requirements

- **ESP32** Development board
- **ESP-IDF** (Espressif IoT Development Framework) environment
- **NimBLE** stack (included in ESP-IDF)
- **FreeRTOS** (comes integrated with ESP-IDF)

## Setup Instructions

1. **Install ESP-IDF:**
   Follow the instructions on the official ESP-IDF [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/) to set up the development environment.

2. **Clone or Download the Code:**
   Clone this repository or download the code files to your local machine.

3. **Configure the Project:**
   Open the project in the ESP-IDF environment and configure it for your specific ESP32 device:
   ```bash
   idf.py menuconfig