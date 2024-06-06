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
#include <ledstrip_manager.hpp>

#define NUM_LEDS1           (30 + 1)
#define NUM_LEDS2           (60 + 1)
#define LED_BRIGHTNESS      100

int main() {
    stdio_init_all();
    lv_init();
    lv_port_disp_init();

    // Initialize CLI
    EmbeddedCliConfig* config = embeddedCliDefaultConfig();
    config->cliBufferSize = 255;
    config->maxBindingCount = 15;
    config->cmdBufferSize = 255;
    EmbeddedCli* cli = embeddedCliNew(config);

    lv_disp_t *disp1 = lv_disp_get_default();
    lv_disp_t *disp2 = lv_disp_get_next(disp1);

    const PicoLed::DataFormat DATA_FORMAT1 = PicoLed::DataFormat::FORMAT_RGB;

    DISPLAY::EyeDisplayDriver driver(disp1, disp2);
    LED::LedStripManager ledStripManager;
    auto strip1 = std::make_unique<LED::LedStripDriver>(pio0, 0, LEDSTRIP1_DATAPIN, NUM_LEDS1, DATA_FORMAT1);
    ledStripManager.addLedStrip(1, std::move(strip1));
    auto strip2 = std::make_unique<LED::LedStripDriver>(pio1, 0, LEDSTRIP2_DATAPIN, NUM_LEDS1, DATA_FORMAT1);
    ledStripManager.addLedStrip(2, std::move(strip2));

    MOTOR::motorManager.addMotor(1, std::make_unique<MOTOR::StepMotorDriver>(MOTOR1_STEP, MOTOR1_DIR, MOTOR1_ENABLE));
    MOTOR::motorManager.addMotor(2, std::make_unique<MOTOR::StepMotorDriver>(MOTOR2_STEP, MOTOR2_DIR, MOTOR2_ENABLE));
    MOTOR::startMotorTask();

    COM::BrainBoardDriver commDriver(UART_BAUD_RATE, UART_TX_PIN, UART_RX_PIN, cli); 

    driver.startTasks();
    commDriver.startTasks();
    ledStripManager.start();

    vTaskStartScheduler();

    while (true) {;}
    return 0;
}