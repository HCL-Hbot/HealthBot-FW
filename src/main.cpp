#include "hardware/structs/rosc.h"
#include <stdio.h>

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include <task.h>
#include "board_defines.h"
#include <hardware/spi.h>
#include <lv_drv_conf.h>
#include <lvgl.h>
#include "lv_port_disp.h"
#include "ui.h"
#include "tusb.h"

#include <eye_driver.hpp>
#include <UART_streamer.hpp>
#include "cli_port.hpp"
#include <ledstrip_driver.hpp>

static inline SERIAL::UART_RTOS_Driver uartStreamer{UART_BAUD_RATE, UART_TX_PIN, UART_RX_PIN};

void uart_task(void *Pvarg) {
    vTaskDelay(1000/portTICK_PERIOD_MS);
    init_cli_interface();
    while (true)
    {
        periodic_cli_handler();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

static void mainTask(void *params) {
    SERIAL::uart_buffer_t rxBuffer;

    while (1) {
        // Check for received UART data without blocking
        if (uartStreamer.uart_check_rx_non_blocking(&rxBuffer, sizeof(rxBuffer), pdMS_TO_TICKS(100))) {
            // Process received UART data
            printf("Received UART data: %s\n", rxBuffer.data);
        } else {
            // Yield to other tasks
            taskYIELD();
        }
    }
}

static void uart_send_task(void *params) {
    const char* message = "Hello, UART World!\n";
    uartStreamer.uart_send_non_blocking(message);

    while (1) {
        uartStreamer.uart_send_non_blocking(message); // Non-blocking send
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

int main() {
    stdio_init_all();
    lv_init();
    lv_port_disp_init();

    lv_disp_t *disp1 = lv_disp_get_default();
    lv_disp_t *disp2 = lv_disp_get_next(disp1);

    DISPLAY::EyeDisplayDriver driver(disp1, disp2);

    xTaskCreate(driver.displayHandler, "display_task", 800, &driver, 1, NULL);
    xTaskCreate(driver.runEyeControlHandle, "uart_task", 1700, &driver, 2, NULL);
    xTaskCreate(uart_task, "uart_task", 360, nullptr, 1, nullptr);

    vTaskStartScheduler();

    return 0;
}
