#ifndef BRAINBOARD_DRIVER_HPP
#define BRAINBOARD_DRIVER_HPP

#include "embedded_cli.h"
#include "UART_streamer.hpp"

#define CLI_PROCESSING_PERIOD 100
#define UART_RECEIVE_TASK_PERIOD 10 

namespace COM {
class BrainBoardDriver {
public:
    BrainBoardDriver(const uint32_t baudRate, const uint8_t txPin, const uint8_t rxPin, EmbeddedCli* cli) :
        uart_driver_(baudRate, txPin, rxPin), cli_(cli) {
        instance_ = this;
        initialize();
        cli->writeChar = writeChar;
        setupCliBindings();
    }

    void initialize() {
            gpio_set_function(17, GPIO_FUNC_UART);
            gpio_set_function(16, GPIO_FUNC_UART);
            uart_init(uart0, 115200);
        // uart_driver_.initialize();
    }

    ~BrainBoardDriver() {
        uart_driver_.deinitialize();
    }

    // void send(const char* data) {
    //     uart_driver_.uart_send_non_blocking(data);
    // }

    // bool receive(char* buffer, uint16_t bufferSize, TickType_t waitTime) {
    //     SERIAL::uart_buffer_t uartBuffer;
    //     if (uart_driver_.uart_check_rx_non_blocking(&uartBuffer, bufferSize, waitTime)) {
    //         strncpy(buffer, uartBuffer.data, uartBuffer.length);
    //         buffer[uartBuffer.length] = '\0'; // Null-terminate the received string
    //         return true;
    //     }
    //     return false;
    // }

    void startCliTask() {
        xTaskCreate(&BrainBoardDriver::cliTask, "CLITask", 600, this, 1, nullptr);
    }

private:
    SERIAL::UART_RTOS_Driver uart_driver_;
    EmbeddedCli* cli_;

    static inline BrainBoardDriver* instance_ = nullptr;

    void setupCliBindings() {
        // Motor Commands
        addCliCommandBinding("M101", "Configure Motor", true, nullptr, configureMotor);
        addCliCommandBinding("M102", "Control Motor", true, nullptr, controlMotor);

        // Display Element Commands
        addCliCommandBinding("D103", "Toggle Display Element", true, nullptr, toggleDisplayElement);
        addCliCommandBinding("D104", "Move Display Element", true, nullptr, moveDisplayElement);
        addCliCommandBinding("D105", "Animate Display Element", true, nullptr, animateDisplayElement);

        // LED Strip Commands
        addCliCommandBinding("L106", "Toggle LED Strip", true, nullptr, toggleLedStrip);
        addCliCommandBinding("L107", "Change LED Color", true, nullptr, changeLedColor);
        addCliCommandBinding("L108", "Set LED Intensity", true, nullptr, setLedIntensity);

        addCliCommandBinding("R109", "Radar Detection", true, nullptr, radarDetection);
        addCliCommandBinding("R110", "Radar Data Streaming", true, nullptr, radarDataStreaming);

        addCliCommandBinding("S111", "Status Check", true, nullptr, statusCheck);
    }

    static inline void writeChar(EmbeddedCli *embeddedCli, char c) {
        uart_putc(uart0, c);
    }

    static void cliTask(void* pvParameters) {
        BrainBoardDriver* driver = static_cast<BrainBoardDriver*>(pvParameters);
        while (true) {
            driver->processCli();
            vTaskDelay(pdMS_TO_TICKS(CLI_PROCESSING_PERIOD));
        }
    }

    bool addCliCommandBinding(const char* name, const char* help, bool tokenizeArgs, void* context, void (*binding)(EmbeddedCli* cli, char* args, void* context)) {
        CliCommandBinding commandBinding = { name, help, tokenizeArgs, context, binding };
        return embeddedCliAddBinding(cli_, commandBinding);
    }

    // Process CLI commands
    void processCli() {
        int c = uart_getc(uart0);
        embeddedCliReceiveChar(cli_, c);
        embeddedCliProcess(cli_);
    }

    // CLI Command Handlers
    static void configureMotor(EmbeddedCli* cli, char* args, void* context) {
        int driverId;
        float current;
        int speed;
        int stepsPerMm;
        sscanf(args, "D%d C%f S%d M%d", &driverId, &current, &speed, &stepsPerMm);
        // Implement motor configuration using the parsed values
        // Example: Motor.configure(driverId, current, speed, stepsPerMm);
        printf("Configuring motor: DriverId=%d, Current=%.2f, Speed=%d, StepsPerMm=%d\n", driverId, current, speed, stepsPerMm);
    }

    static void controlMotor(EmbeddedCli* cli, char* args, void* context) {
        int yaw;
        int pitch;
        int speed;
        sscanf(args, "Y%d P%d S%d", &yaw, &pitch, &speed);
        // Implement motor control using the parsed values
        // Example: Motor.control(yaw, pitch, speed);
        printf("Controlling motor: Yaw=%d, Pitch=%d, Speed=%d\n", yaw, pitch, speed);
    }

    static void toggleDisplayElement(EmbeddedCli* cli, char* args, void* context) {
        int elementId;
        char state[4];
        sscanf(args, "I%d S%s", &elementId, state);
        // Implement display element toggle using the parsed values
        // Example: Display.toggle(elementId, state);
        printf("Toggling display element: ElementId=%d, State=%s\n", elementId, state);
    }

    static void moveDisplayElement(EmbeddedCli* cli, char* args, void* context) {
        int elementId;
        int xCoord;
        int yCoord;
        sscanf(args, "I%d X%d Y%d", &elementId, &xCoord, &yCoord);
        // Implement display element move using the parsed values
        // Example: Display.move(elementId, xCoord, yCoord);
        printf("Moving display element: ElementId=%d, X=%d, Y=%d\n", elementId, xCoord, yCoord);
    }

    static void animateDisplayElement(EmbeddedCli* cli, char* args, void* context) {
        int elementId;
        char animation[16];
        sscanf(args, "I%d A%s", &elementId, animation);
        // Implement display element animation using the parsed values
        // Example: Display.animate(elementId, animation);
        printf("Animating display element: ElementId=%d, Animation=%s\n", elementId, animation);
    }

    static void toggleLedStrip(EmbeddedCli* cli, char* args, void* context) {
        char state[4];
        sscanf(args, "S%s", state);
        // Implement LED strip toggle using the parsed values
        // Example: LEDStrip.toggle(state);
        printf("Toggling LED strip: State=%s\n", state);
    }

    static void changeLedColor(EmbeddedCli* cli, char* args, void* context) {
        char color[8];
        sscanf(args, "C%s", color);
        // Implement LED color change using the parsed values
        // Example: LEDStrip.changeColor(color);
        printf("Changing LED color: Color=%s\n", color);
    }

    static void setLedIntensity(EmbeddedCli* cli, char* args, void* context) {
        int intensity;
        sscanf(args, "V%d", &intensity);
        // Implement LED intensity change using the parsed values
        // Example: LEDStrip.setIntensity(intensity);
        printf("Setting LED intensity: Intensity=%d\n", intensity);
    }

    static void radarDetection(EmbeddedCli* cli, char* args, void* context) {
        // Implement radar detection
        // Example: Radar.detect();
        printf("Radar detection triggered.\n");
    }

    static void radarDataStreaming(EmbeddedCli* cli, char* args, void* context) {
        char state[6];
        sscanf(args, "S%s", state);
        // Implement radar data streaming using the parsed values
        // Example: Radar.stream(state);
        printf("Radar data streaming: State=%s\n", state);
    }

    static void statusCheck(EmbeddedCli* cli, char* args, void* context) {
        char component[8];
        sscanf(args, "C%s", component);
        // Implement status check for the specified component
        // Example: Status.check(component);
        printf("Status check: Component=%s\n", component);
    }
}; // class BrainBoardDriver
} // namespace COM
#endif // BRAINBOARD_DRIVER_HPP