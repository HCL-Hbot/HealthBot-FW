#ifndef BRAINBOARD_DRIVER_HPP
#define BRAINBOARD_DRIVER_HPP

#include "command_defines.hpp"
#include "command_handlers.hpp"

#include "embedded_cli.h"
#include "UART_streamer.hpp"
#include "board_defines.h"
#include <step_motor.hpp>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define CLI_PROCESSING_PERIOD 100
#define UART_RECEIVE_TASK_PERIOD 10

namespace COM {

// Global, but namespace limited Queues:
QueueHandle_t motorCommandQueue;

class BrainBoardDriver {
public:
    BrainBoardDriver(const uint32_t baudRate, const uint8_t txPin, const uint8_t rxPin, EmbeddedCli* cli) :
        uart_driver_(baudRate, txPin, rxPin), cli_(cli) {
        instance_ = this;
        initialize();
        cli->writeChar = writeChar;
        setupCliBindings();

        // Queeue Creation: 
        motorCommandQueue = xQueueCreate(10, sizeof(MotorCommand));

        if (motorCommandQueue == nullptr || ledStripCommandQueue == nullptr) {
            printf("Failed to create command queues\n");
        }
    }

    void initialize() {
        uart_driver_.initialize();
    }

    ~BrainBoardDriver() {
        uart_driver_.deinitialize();
    }

    void startTasks() {
        xTaskCreate(&BrainBoardDriver::cliTask, "CLITask", 600, this, 1, nullptr);
        xTaskCreate(motorTask, "MotorTask", 600, motorCommandQueue, 1, nullptr);
    }

    void writeByte(uint8_t byte) {
        uart_driver_.writeByte(byte);
    }

private:
    SERIAL::UART_RTOS_Driver uart_driver_;
    EmbeddedCli* cli_;

    static inline BrainBoardDriver* instance_ = nullptr;

    void setupCliBindings() {
        addCliCommandBinding(cli_, "M101", "Configure Motor <MotorID> <Speed>", true, nullptr, configureMotor);
        addCliCommandBinding(cli_, "M102", "Control Motor <MotorID> <start|stop>", true, nullptr, controlMotor);
    }

    static inline void writeChar(EmbeddedCli* embeddedCli, char c) {
        instance_->writeByte(static_cast<uint8_t>(c));
    }

    static void cliTask(void* pvParameters) {
        BrainBoardDriver* driver = static_cast<BrainBoardDriver*>(pvParameters);
        SERIAL::uart_buffer_t rxBuffer;

        while (true) {
            if (driver->uart_driver_.uart_check_rx_non_blocking(&rxBuffer, sizeof(rxBuffer), pdMS_TO_TICKS(UART_RECEIVE_TASK_PERIOD))) {
                for (uint16_t i = 0; i < rxBuffer.length; i++) {
                    embeddedCliReceiveChar(driver->cli_, rxBuffer.data[i]);
                }
                embeddedCliProcess(driver->cli_);
            } else {
                taskYIELD();
            }
            vTaskDelay(pdMS_TO_TICKS(CLI_PROCESSING_PERIOD));
        }
    }

    static void motorTask(void* pvParameters) {
        QueueHandle_t motorCommandQueue = (QueueHandle_t)pvParameters;
        MotorCommand command;

        while (true) {
            if (xQueueReceive(motorCommandQueue, &command, portMAX_DELAY) == pdPASS) {
                switch (command.type) {
                    case MotorCommand::CONFIGURE:
                        printf("Configuring motor %d to speed %d RPM\n", command.motorId, command.speed);
                        // Add motor configuration logic here
                        break;
                    case MotorCommand::CONTROL:
                        if (command.start) {
                            printf("Starting motor %d\n", command.motorId);
                            // Add motor start logic here
                        } else {
                            printf("Stopping motor %d\n", command.motorId);
                            // Add motor stop logic here
                        }
                        break;
                }
            }
        }
    }
}; // class BrainBoardDriver
} // namespace COM
#endif // BRAINBOARD_DRIVER_HPP