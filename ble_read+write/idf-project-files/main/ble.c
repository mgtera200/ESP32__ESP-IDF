/****************************************************************************************
 * Author: Mostafa Gamal Tera
 * Date: December 2024
 * Project: BLE Server Example
 * Description: This code demonstrates a basic BLE server implementation using NimBLE on the ESP32.
 * 
 * Notes: 
 * - Includes functionality for advertising, reading, and writing data.
 * - Utilizes FreeRTOS for task management.
 * 
 ****************************************************************************************/


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "sdkconfig.h"

// Tag for logging
char *TAG = "BLE-TERA";

// BLE address type
uint8_t ble_addr_type;

// Function prototype for starting BLE advertising
void ble_app_advertise(void);

// Callback function for writing data to the server
// This is triggered when the client writes to a characteristic
static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    // Log and print the data received from the client
    printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);
    return 0;
}

// Callback function for reading data from the server
// This is triggered when the client reads from a characteristic
static int device_read(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    // Send the data "0xDEAD" (two bytes in hexadecimal)
    uint8_t data[] = {0xDE, 0xAD};  // Create a byte array for 0xDEAD
    os_mbuf_append(ctxt->om, data, sizeof(data));  // Append the byte array to the response buffer
    return 0;
}


// GATT service and characteristic definitions
// This defines a primary service with two characteristics: one for reading and one for writing
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID16_DECLARE(0x180),                 // Primary service UUID
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID16_DECLARE(0xDEAD),           // Characteristic UUID for reading
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = device_read},                   // Callback for read access
         {.uuid = BLE_UUID16_DECLARE(0xFEF4),           // Characteristic UUID for writing
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = device_write},                  // Callback for write access
         {0}}},                                        // End of characteristic list
    {0}};                                              // End of service list

// GAP event handler for managing BLE events
// Handles connection and advertising events
static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT: // Handle connection events
        ESP_LOGI("GAP", "BLE GAP EVENT CONNECT %s", event->connect.status == 0 ? "OK!" : "FAILED!");
        if (event->connect.status != 0) // If connection failed, restart advertising
        {
            ble_app_advertise();
        }
        break;
    case BLE_GAP_EVENT_ADV_COMPLETE: // Handle advertising completion
        ESP_LOGI("GAP", "BLE GAP EVENT");
        ble_app_advertise(); // Restart advertising
        break;
    default:
        break;
    }
    return 0;
}

// Starts BLE advertising
// Configures GAP advertising parameters and fields
void ble_app_advertise(void)
{
    struct ble_hs_adv_fields fields;
    const char *device_name;
    memset(&fields, 0, sizeof(fields));

    // Set device name in advertising payload
    device_name = ble_svc_gap_device_name(); // Retrieve the BLE device name
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;

    ble_gap_adv_set_fields(&fields); // Configure advertising fields

    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));

    // Configure advertising parameters
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; // Undirected connectable mode
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN; // General discoverable mode

    ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
}

// Synchronization callback for the BLE stack
// Configures the device's address and starts advertising
void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type); // Determine address type automatically
    ble_app_advertise();                     // Start advertising
}

// Task for the NimBLE host
// This runs the BLE host and handles BLE protocol processing
void host_task(void *param)
{
    nimble_port_run(); // Starts the NimBLE host task
}

// Main application entry point
void app_main()
{
    // Initialize Non-Volatile Storage (NVS)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());  // Erase NVS if it has a problem
        ret = nvs_flash_init();  // Re-initialize NVS
    }
    ESP_ERROR_CHECK(ret);  // Check if initialization was successful

    // Initialize the NimBLE stack
    nimble_port_init();
    ble_svc_gap_device_name_set("BLE-TERA"); // Set device name
    ble_svc_gap_init();                      // Initialize GAP service
    ble_svc_gatt_init();                     // Initialize GATT service
    ble_gatts_count_cfg(gatt_svcs);          // Configure GATT services
    ble_gatts_add_svcs(gatt_svcs);           // Add GATT services

    // Set the synchronization callback
    ble_hs_cfg.sync_cb = ble_app_on_sync;

    // Start the NimBLE host task using FreeRTOS
    nimble_port_freertos_init(host_task);
}
