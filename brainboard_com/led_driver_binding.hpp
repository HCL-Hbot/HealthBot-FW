#ifndef LED_DRIVER_BINDING_HPP
#define LED_DRIVER_BINDING_HPP

#include "embedded_cli.h"
#include "command_defines.hpp"

#include "FreeRTOS.h"
#include "queue.h"

namespace COM {

extern QueueHandle_t leddriver_command_queue;

/* 1 - COMMAND DEFINES */
enum class LedCommandType {
    SET_LED             = 0,
    GET_LED             = 1,
};

struct LedDriverCommand {
    LedCommandType type;
    int ledId;
    int turnOn;
};

/* 2 - HANDLERS */
static void handleSetLed(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);

    if (argCount == 2) {
        const char* ledId = embeddedCliGetToken(args, 1);
        const char* turnOn = embeddedCliGetToken(args, 2);

        const int ledstripId = atoi(ledId);
        const int led_state = atoi(turnOn);

        if (led_state == 0 || led_state == 1) {
            LedDriverCommand command;
            command.type = LedCommandType::SET_LED;
            command.ledId = ledstripId;
            command.turnOn = led_state;
            xQueueSend(leddriver_command_queue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for SET_LED command\n");
        }
    } else {
        printf("Invalid argument count for SET_LED command\n");
    }
}

/* 3 - CLI BINDINGS */
static void addLedCliBindings(EmbeddedCli* cli) {
    addCliCommandBinding(cli, "L101",
        "Set LED: LEDSTRIP SET_LED <LedstripID> <ON/OFF>", 
        true, nullptr, handleSetLed);
}
} // namespace COM
#endif // LED_DRIVER_BINDING_HPP