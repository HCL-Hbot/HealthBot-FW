#ifndef BRAINBOARD_DRIVER_HPP
#define BRAINBOARD_DRIVER_HPP

#include "command_defines.hpp"

#include "embedded_cli.h"
#include "UART_streamer.hpp"
#include "board_defines.h"

#include <eye_driver.hpp>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "motor_driver_binding.hpp"

#define CLI_PROCESSING_PERIOD       100
#define UART_RECEIVE_TASK_PERIOD    100

namespace COM {

// Global, but namespace limited Queues:
QueueHandle_t motorCommandQueue;
QueueHandle_t eyeControlCommandQueue;
QueueHandle_t ledStripCommandQueue;

class BrainBoardDriver {
public:
    BrainBoardDriver(const uint32_t baudRate, const uint8_t txPin, 
    const uint8_t rxPin, EmbeddedCli* cli) :
        uart_driver_(baudRate, txPin, rxPin), cli_(cli) {
        instance_ = this;
        initialize();
        cli->writeChar = writeChar;
        setupCliBindings();

        // Queeue Creation: 
        motorCommandQueue = xQueueCreate(10, sizeof(MotorCommand));

        if (motorCommandQueue == nullptr) {
            printf("Failed to create MOTOR command queue.\n");
        }
    }

    ~BrainBoardDriver() {
        uart_driver_.deinitialize();
    }

    void initialize() {
        uart_driver_.initialize();
    }

    void startTasks() {
        xTaskCreate(&BrainBoardDriver::cliTaskHandle, "EMB-CLI-Handler", 700, this, 1, nullptr);
    }

    void writeByte(uint8_t byte) {
        uart_driver_.writeByte(byte);
    }

private:
    SERIAL::UART_RTOS_Driver uart_driver_;
    EmbeddedCli* cli_;
    static inline BrainBoardDriver* instance_ = nullptr;

    void setupCliBindings() {
        addMotorCliBindings(cli_);
    }

    static inline void writeChar(EmbeddedCli* embeddedCli, char c) {
        instance_->writeByte(static_cast<uint8_t>(c));
    }

    static void cliTaskHandle(void* pvParameters) {
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
        }
    }
}; // class BrainBoardDriver
} // namespace COM
#endif // BRAINBOARD_DRIVER_HPP