#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/twai.h"

#define TX_PIN 21
#define RX_PIN 20

void app_main(void) {
    vTaskDelay(pdMS_TO_TICKS(6000));
    printf("\n=== CAN EMPFAENGER STARTET ===\n");

    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_PIN, RX_PIN, TWAI_MODE_LISTEN_ONLY);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_125KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        twai_start();
        printf("Status: Warte auf Rohdaten (125k)...\n");
    } else {
        printf("Status: FEHLER beim Treiber-Install!\n");
        return;
    }

    twai_message_t msg;
    while (1) {
        esp_err_t res = twai_receive(&msg, pdMS_TO_TICKS(1000));
        
        if (res == ESP_OK) {
            printf("Empfangen <- ID: %03lx | DLC: %d | Data: ", msg.identifier, msg.data_length_code);
            for (int i = 0; i < msg.data_length_code; i++) {
                printf("%02x ", msg.data[i]);
            }
            printf("\n");
        } else {
            printf("Empfangen <- Fehler beim Empfang (0x%x)\n", res);
        }
    }
}