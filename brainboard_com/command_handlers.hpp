#ifndef COMMAND_HANDLERS_HPP
#define COMMAND_HANDLERS_HPP

#include <embedded_cli.h>

#include <stdio.h>
#include <string.h>

namespace COM {
    extern QueueHandle_t motorCommandQueue;
    extern QueueHandle_t ledStripCommandQueue;

    static bool addCliCommandBinding(EmbeddedCli* cli, const char* name, const char* help, bool tokenizeArgs, void* context, void (*binding)(EmbeddedCli* cli, char* args, void* context)) {
        CliCommandBinding commandBinding = { name, help, tokenizeArgs, context, binding };
        return embeddedCliAddBinding(cli, commandBinding);
    }

    static void configureMotor(EmbeddedCli* cli, char* args, void* context) {
        uint16_t argCount = embeddedCliGetTokenCount(args);
        int motorId = -1;
        int speed = -1;

        for (uint16_t i = 1; i <= argCount; ++i) {
            const char* token = embeddedCliGetToken(args, i);
            if (token[0] == 'D') {
                motorId = atoi(&token[1]);
            } else if (token[0] == 'S') {
                speed = atoi(&token[1]);
            }
        }

        if (motorId != -1 && speed != -1) {
            MotorCommand command;
            command.type = MotorCommand::CONFIGURE;
            command.motorId = motorId;
            command.speed = speed;

            if (xQueueSend(motorCommandQueue, &command, pdMS_TO_TICKS(10)) != pdPASS) {
                printf("Failed to send configure command\r\n");
            }
        } else {
            printf("Invalid arguments for M101 command\r\n");
        }
    }

    static void controlMotor(EmbeddedCli* cli, char* args, void* context) {
        uint16_t argCount = embeddedCliGetTokenCount(args);
        if (argCount == 2) {
            const char* motorIdStr = embeddedCliGetToken(args, 1);
            const char* actionStr = embeddedCliGetToken(args, 2);
            MotorCommand command;
            command.type = MotorCommand::CONTROL;
            command.motorId = atoi(motorIdStr);
            command.start = (strcmp(actionStr, "start") == 0);

            if (xQueueSend(motorCommandQueue, &command, pdMS_TO_TICKS(10)) != pdPASS) {
                printf("Failed to send control command\r\n");
            }
        } else {
            printf("Invalid arguments for M102 command\r\n");
        }
    }
} // namespace COM
#endif // COMMAND_HANDLERS_HPP  