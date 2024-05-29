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

int main() {
    stdio_init_all();
    lv_init();
    lv_port_disp_init();

    lv_disp_t *disp1 = lv_disp_get_default();
    lv_disp_t *disp2 = lv_disp_get_next(disp1);

    DISPLAY::EyeDisplayDriver driver(disp1, disp2);

    xTaskCreate(driver.displayHandler, "display_task", 800, &driver, 1, NULL);
    xTaskCreate(driver.runEyeControlHandle, "uart_task", 1700, &driver, 2, NULL);

    vTaskStartScheduler();

    return 0;
}
