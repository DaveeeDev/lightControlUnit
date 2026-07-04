#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/twai.h"

#define TX_PIN 10
#define RX_PIN 9

void app_main(void) {
    vTaskDelay(pdMS_TO_TICKS(6000));
    printf("\n=== CAN SENDER STARTET ===\n");

    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_PIN, RX_PIN, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_125KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        twai_start();
        printf("Status: Treiber installiert und gestartet (125k).\n");
    } else {
        printf("Status: FEHLER beim Treiber-Install!\n");
        return;
    }

    uint8_t zaehler = 0;
    while (1) {
        twai_message_t msg = {
            .identifier = 0x123,
            .data_length_code = 3,
            .data = {0xAA, 0xBB, zaehler}
        };

        if (twai_transmit(&msg, pdMS_TO_TICKS(1000)) == ESP_OK) {
            printf("Sende -> ID: 123 | Byte2: %02x\n", zaehler);
            zaehler++;
        } else {
            printf("Sende -> FEHLGESCHLAGEN (Kein ACK / Bus Error)\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}