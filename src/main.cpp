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
#include <Effects/Fade.hpp>
#include "PicoLedEffect.hpp"

#include <brainboard_driver.hpp>
#include <step_motor_manager.hpp>
#include <motor_task.hpp>

#define NUM_LEDS1 30
#define NUM_LEDS2 60
#define LED_BRIGHTNESS 100

int main() {
    stdio_init_all();
    lv_init();
    lv_port_disp_init();

    // Initialize CLI
    EmbeddedCliConfig* config = embeddedCliDefaultConfig();
    EmbeddedCli* cli = embeddedCliNew(config);

    lv_disp_t *disp1 = lv_disp_get_default();
    lv_disp_t *disp2 = lv_disp_get_next(disp1);

    DISPLAY::EyeDisplayDriver* driver = new DISPLAY::EyeDisplayDriver(disp1, disp2);
    LED::LedStripController* ledStrips = new LED::LedStripController(LEDSTRIP1_DATAPIN, NUM_LEDS1, LEDSTRIP2_DATAPIN, NUM_LEDS2);

    // DISPLAY::EyeDisplayDriver driver(disp1, disp2);
    //xTaskCreate(driver->displayHandler, "display_task", 800, driver, 2, NULL);

    // MOTOR::motorManager.addMotor(1, std::make_unique<MOTOR::StepMotorDriver>(MOTOR1_STEP, MOTOR1_DIR, MOTOR1_ENABLE));
    // MOTOR::motorManager.addMotor(2, std::make_unique<MOTOR::StepMotorDriver>(MOTOR2_STEP, MOTOR2_DIR, MOTOR2_ENABLE));
    // MOTOR::startMotorTask();

    COM::BrainBoardDriver commDriver(UART_BAUD_RATE, UART_TX_PIN, UART_RX_PIN, cli); 
    DISPLAY::startEyeDisplayDriverTask(driver);
    commDriver.startTasks();

    vTaskStartScheduler();

    while (true) {;}

    delete driver;
    delete ledStrips;

    return 0;
}