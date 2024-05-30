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
#include <brainboard_driver.hpp>

int main() {
    stdio_init_all();
    lv_init();
    lv_port_disp_init();

    lv_disp_t *disp1 = lv_disp_get_default();
    lv_disp_t *disp2 = lv_disp_get_next(disp1);

    DISPLAY::EyeDisplayDriver driver(disp1, disp2);

    // Initialize CLI
    EmbeddedCliConfig* config = embeddedCliDefaultConfig();
    EmbeddedCli* cli = embeddedCliNew(config);

    COM::BrainBoardDriver commDriver(UART_BAUD_RATE, UART_TX_PIN, UART_RX_PIN, cli); 

    // Start CLI processing task
    commDriver.startTasks();

    vTaskStartScheduler();

    while (true) {;}
    return 0;
}