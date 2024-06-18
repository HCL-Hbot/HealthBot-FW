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

#include <main.hpp>

int main() {
    stdio_init_all();
    timer_hw->dbgpause = 0; // Disable timer pausing during debug

    /* LVGL Setup */
    lv_init();
    lv_port_disp_init();
    lv_disp_t *disp1 = lv_disp_get_default();       // Display 1 
    lv_disp_t *disp2 = lv_disp_get_next(disp1);     // Display 2

    /* Embedded CLI Setup */
    EmbeddedCliConfig* config = embeddedCliDefaultConfig();
    config->cliBufferSize = 255;
    config->maxBindingCount = 20;
    config->cmdBufferSize = 300;
    EmbeddedCli* cli = embeddedCliNew(config);

    /* IO Expander Setup */
    i2c_inst_t *i2c = i2c1;
    uint8_t address = 0x20; // A2 & A1 are L in the PCB so Addr 32DEC. 
    uint8_t sda_pin = IO_EXPANDER_SDA;    // Replace with your SDA pin
    uint8_t scl_pin = IO_EXPANDER_SCL;    // Replace with your SCL pin
    uint32_t baudrate = 100'000;
    // singleton instance of the IO expander: 
    EXPANDER::TCA9534XXX &expander = EXPANDER::TCA9534XXX::getInstance(i2c, address, sda_pin, scl_pin, baudrate);
    // Initialize the expander
    if (! expander.init()) {
        printf("Failed to initialize TCA9534DWR\n");
        return 1;
    }

    /* LEDSTRIP Format Setup */
    const PicoLed::DataFormat DATA_FORMAT1 = PicoLed::DataFormat::FORMAT_RGB;
    const PicoLed::DataFormat DATA_FORMAT2 = PicoLed::DataFormat::FORMAT_GRB;

    /* Driver Objects: */
    DISPLAY::EyeDisplayDriver eyes_driver(disp1, disp2);
    LED::LedDriver cheek_driver(expander, PEXP5_LED_CHEECKS); 
    COM::BrainBoardDriver communication_driver(UART_BAUD_RATE, UART_TX_PICO, UART_RX_PICO, cli); 
    LED::LedStripManager ledstrip_manager;
    MOTOR::MotorManager motor_manager;

    // LEDSTRIP Managers setup: 
    auto strip1 = std::make_unique<LED::LedStripDriver>(pio0, 0, LEDSTRIP1_DATAPIN, NUM_LEDS1, DATA_FORMAT1);
    auto strip2 = std::make_unique<LED::LedStripDriver>(pio1, 0, LEDSTRIP2_DATAPIN, NUM_LEDS2, DATA_FORMAT2);
    ledstrip_manager.addLedStrip(CLI_LEDSTRIP_ONE_REF, std::move(strip1));
    ledstrip_manager.addLedStrip(CLI_LEDSTRIP_TWO_REF, std::move(strip2));
    // MOTOR Manager setup:
    motor_manager.addMotor(CLI_MOTOR_ONE_REF, std::make_unique<MOTOR::StepMotorDriver>(expander, MOTOR1_STEP, MOTOR1_DIR, PEXP0_MOTOR1_ENABLE));
    motor_manager.addMotor(CLI_MOTOR_TWO_REF, std::make_unique<MOTOR::StepMotorDriver>(expander, MOTOR2_STEP, MOTOR2_DIR, PEXP1_MOTOR2_ENABLE));
    
    /* DRIVER TASKS START: */
    motor_manager.startMotorTask();
    eyes_driver.startTasks();
    ledstrip_manager.start();
    cheek_driver.start(&cheek_driver);
    communication_driver.startTasks();
    RADAR::startRadarTasks(&communication_driver);
    // Schedule the tasks: 
    vTaskStartScheduler();

    while (true) {;}

    /* Cleanup of Dynamic Memory: */
    embeddedCliFree(cli);
    delete config;
    delete cli;
    delete disp1;
    delete disp2;
    return 0;
}