#ifndef BRAINBOARD_DRIVER_HPP
#define BRAINBOARD_DRIVER_HPP

#include "command_defines.hpp"
#include "command_handlers.hpp"

#include "embedded_cli.h"
#include "UART_streamer.hpp"
#include "board_defines.h"

#include <step_motor.hpp>
#include <eye_driver.hpp>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define CLI_PROCESSING_PERIOD       100
#define UART_RECEIVE_TASK_PERIOD    10

namespace COM {

// Global, but namespace limited Queues:
QueueHandle_t motorCommandQueue;
QueueHandle_t eyeControlCommandQueue;
QueueHandle_t ledStripCommandQueue;

class BrainBoardDriver {
public:
    BrainBoardDriver(const uint32_t baudRate, const uint8_t txPin, 
    const uint8_t rxPin, EmbeddedCli* cli, 
    DISPLAY::EyeDisplayDriver* eyeDisplayDriver,
    LED::LedStripController* ledStripController) :
        uart_driver_(baudRate, txPin, rxPin), cli_(cli), eyeDisplayDriver_(eyeDisplayDriver) {
        instance_ = this;
        initialize();
        cli->writeChar = writeChar;
        setupCliBindings();

        // Queeue Creation: 
        motorCommandQueue = xQueueCreate(10, sizeof(MotorCommand));
        eyeControlCommandQueue = xQueueCreate(10, sizeof(EyeControlCommand));
        ledStripCommandQueue = xQueueCreate(10, sizeof(LEDStripCommand));

        if (motorCommandQueue == nullptr || eyeControlCommandQueue == nullptr || ledStripCommandQueue == nullptr) {
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
        xTaskCreate(&BrainBoardDriver::cliTaskHandle, "EMB-CLI-Handler", 600, this, 1, nullptr);
        xTaskCreate(processMotorRequests, "MotorTask", 600, motorCommandQueue, 1, nullptr);
        xTaskCreate(processEyeRequests, "EyeTask", 600, eyeControlCommandQueue, 1, nullptr);
        xTaskCreate(processLEDStripRequests, "LEDStripTask", 600, ledStripCommandQueue, 1, nullptr);
    }

    void writeByte(uint8_t byte) {
        uart_driver_.writeByte(byte);
    }

private:
    SERIAL::UART_RTOS_Driver uart_driver_;
    EmbeddedCli* cli_;

    static inline BrainBoardDriver* instance_ = nullptr;
    DISPLAY::EyeDisplayDriver* eyeDisplayDriver_;
    LED::LedStripController* ledStripController_;


    void setupCliBindings() {

        addCliCommandBinding(cli_, "M101", 
            "Configure Motor D<MotorID> S<Speed>", 
            true, nullptr, interpretMotorConfigureRequest);
        addCliCommandBinding(cli_, "M102", "Control Motor <MotorID> <start|stop>", 
            true, nullptr, interpretMotorControlRequest);

        addCliCommandBinding(cli_, "E101", 
            "Move Eye <EyeID> <X> <Y> <Duration>", 
            true, nullptr, interpretEyeMovementRequest);
        addCliCommandBinding(cli_, "E102", 
            "Animate Eye <EyeID> <AnimType> <X> <Y> <Duration>", 
            true, nullptr, interpretEyeAnimationRequest);
            
        addCliCommandBinding(cli_, "L101", 
            "Set LED Strip Color <R> <G> <B>", 
            true, nullptr, interpretLEDStripSetColor);
        addCliCommandBinding(cli_, "L102", 
            "Set LED Strip Brightness <Brightness>", 
            true, nullptr, interpretLEDStripSetBrightness);
        addCliCommandBinding(cli_, "L103", 
            "Fade LED Strip Color <R> <G> <B> <Delay>", 
            true, nullptr, interpretLEDStripFade);
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
            vTaskDelay(pdMS_TO_TICKS(CLI_PROCESSING_PERIOD));
        }
    }

    static void processMotorRequests(void* pvParameters) {
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

    static void processEyeRequests(void* pvParameters) {
        QueueHandle_t eyeControlCommandQueue = (QueueHandle_t)pvParameters;
        EyeControlCommand command;

        while (true) {
            if (xQueueReceive(eyeControlCommandQueue, &command, portMAX_DELAY) == pdPASS) {
                if (instance_ && instance_->eyeDisplayDriver_) {
                    instance_->eyeDisplayDriver_->handleEyeCommand(command);
                } else {
                    printf("Eye display driver not initialized\n");
                }
            }
        }
    }

    static void processLEDStripRequests(void* pvParameters) {
        QueueHandle_t ledStripCommandQueue = (QueueHandle_t)pvParameters;
        LEDStripCommand command;

        LED::LedStripController* ledStrips = static_cast<LED::LedStripController*>(pvParameters);

        while (true) {
            if (xQueueReceive(ledStripCommandQueue, &command, portMAX_DELAY) == pdPASS) {
                switch (command.type) {
                    case LEDStripCommand::SET_COLOR:
                        ledStrips->fill(command.color);
                        ledStrips->show();
                        break;
                    case LEDStripCommand::SET_BRIGHTNESS:
                        ledStrips->setBrightness(command.brightness);
                        ledStrips->show();
                        break;
                    case LEDStripCommand::FADE:
                        ledStrips->effectFade(command.color, command.fadeDelay);
                        break;
                }
            }
        }
    }
}; // class BrainBoardDriver
} // namespace COM
#endif // BRAINBOARD_DRIVER_HPP