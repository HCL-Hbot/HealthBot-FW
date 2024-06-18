/*
 * Copyright 2024 JUNE - HAN University of Applied Sciences, Health Concept Lab
 * Project HealthBot
 * Authors: Victor Hogeweij, Marelle Vink and Richard Kroesen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
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
#include <ledstrip_manager.hpp>

#include <bgt60ltr11XXX_driver.hpp>
#include <expander_driver.hpp>
#include <radar_task.hpp>
#include <led_driver.hpp>

#define NUM_LEDS1           (30 + 1)
#define NUM_LEDS2           (60 + 1)
#define LED_BRIGHTNESS      100

int main() {
    stdio_init_all();

    // Disable timer pausing during debug
    timer_hw->dbgpause = 0;

    lv_init();
    lv_port_disp_init();

    // Initialize CLI
    EmbeddedCliConfig* config = embeddedCliDefaultConfig();
    config->cliBufferSize = 255;
    config->maxBindingCount = 20;
    config->cmdBufferSize = 300;
    EmbeddedCli* cli = embeddedCliNew(config);

    lv_disp_t *disp1 = lv_disp_get_default();
    lv_disp_t *disp2 = lv_disp_get_next(disp1);

    /* IO Expander Test*/
    i2c_inst_t *i2c = i2c1;
    uint8_t address = 0x20; // A2 & A1 are L in the PCB so Addr 32DEC. 
    uint8_t sda_pin = IO_EXPANDER_SDA;    // Replace with your SDA pin
    uint8_t scl_pin = IO_EXPANDER_SCL;    // Replace with your SCL pin
    uint32_t baudrate = 100'000;

    EXPANDER::TCA9534XXX &expander 
        = EXPANDER::TCA9534XXX::getInstance(i2c, address, sda_pin, scl_pin, baudrate);

    // Initialize the expander
    if (!expander.init()) {
        printf("Failed to initialize TCA9534DWR\n");
        return 1;
    }

    const PicoLed::DataFormat DATA_FORMAT1 = PicoLed::DataFormat::FORMAT_RGB;
    const PicoLed::DataFormat DATA_FORMAT2 = PicoLed::DataFormat::FORMAT_GRB;

    DISPLAY::EyeDisplayDriver driver(disp1, disp2);
    LED::LedStripManager ledStripManager;
    LED::LedDriver ledDriver(expander, PEXP5_LED_CHEECKS); 
    MOTOR::MotorManager motorManager;

    auto strip1 = std::make_unique<LED::LedStripDriver>(pio0, 0, LEDSTRIP1_DATAPIN, NUM_LEDS1, DATA_FORMAT1);
    auto strip2 = std::make_unique<LED::LedStripDriver>(pio1, 0, LEDSTRIP2_DATAPIN, NUM_LEDS2, DATA_FORMAT2);
    
    ledStripManager.addLedStrip(1, std::move(strip1));
    ledStripManager.addLedStrip(2, std::move(strip2));
    motorManager.addMotor(1, std::make_unique<MOTOR::StepMotorDriver>(expander, MOTOR1_STEP, MOTOR1_DIR, PEXP0_MOTOR1_ENABLE));
    motorManager.addMotor(2, std::make_unique<MOTOR::StepMotorDriver>(expander, MOTOR2_STEP, MOTOR2_DIR, PEXP1_MOTOR2_ENABLE));
    
    motorManager.startMotorTask();
    driver.startTasks();
    ledStripManager.start();
    ledDriver.start(&ledDriver);

    COM::BrainBoardDriver commDriver(UART_BAUD_RATE, UART_TX_PICO, UART_RX_PICO, cli); 
    commDriver.startTasks();
    RADAR::startRadarTasks(&commDriver);

    vTaskStartScheduler();

    while (true) {;}

    delete config;
    delete cli;
    delete disp1;
    delete disp2;

    return 0;
}