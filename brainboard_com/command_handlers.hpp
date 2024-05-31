#ifndef COMMAND_HANDLERS_HPP
#define COMMAND_HANDLERS_HPP

#include <stdio.h>
#include <string.h>
#include <embedded_cli.h>
#include <PicoLed.hpp>
#include <Effects/Fade.hpp>

namespace COM {
    extern QueueHandle_t motorCommandQueue;
    extern QueueHandle_t eyeControlCommandQueue;
    extern QueueHandle_t ledStripCommandQueue;

    static bool addCliCommandBinding(EmbeddedCli* cli, const char* name, const char* help, bool tokenizeArgs, void* context, void (*binding)(EmbeddedCli* cli, char* args, void* context)) {
        CliCommandBinding commandBinding = { name, help, tokenizeArgs, context, binding };
        return embeddedCliAddBinding(cli, commandBinding);
    }

/*=== MOTOR === */
    static void interpretMotorConfigureRequest(EmbeddedCli* cli, char* args, void* context) {
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

    static void interpretMotorControlRequest(EmbeddedCli* cli, char* args, void* context) {
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
/*=== MOTOR     === */
/*=== DISPLAYS  ===*/
    static void interpretEyeMovementRequest(EmbeddedCli* cli, char* args, void* context) {
        const uint16_t argCount = embeddedCliGetTokenCount(args);

        if (argCount == 4) {
            const char* eyeIdStr = embeddedCliGetToken(args, 1);
            const char* xStr = embeddedCliGetToken(args, 2);
            const char* yStr = embeddedCliGetToken(args, 3);
            const char* durationStr = embeddedCliGetToken(args, 4);
            EyeControlCommand command;

            command.type = EyeControlCommand::MOVE;
            command.eyeId = atoi(eyeIdStr);
            command.x = atoi(xStr);
            command.y = atoi(yStr);
            command.duration = atoi(durationStr);

            if (xQueueSend(eyeControlCommandQueue, &command, pdMS_TO_TICKS(10)) != pdPASS) {
                printf("Failed to send eye move command\r\n");
            }

        } else {
            printf("Invalid arguments for E101 command\r\n");
        }
    }

    static void interpretEyeAnimationRequest(EmbeddedCli* cli, char* args, void* context) {
        const uint16_t argCount = embeddedCliGetTokenCount(args);

        if (argCount == 5) {
            const char* eyeIdStr = embeddedCliGetToken(args, 1);
            const char* animTypeStr = embeddedCliGetToken(args, 2);
            const char* xStr = embeddedCliGetToken(args, 3);
            const char* yStr = embeddedCliGetToken(args, 4);
            const char* durationStr = embeddedCliGetToken(args, 5);
            EyeControlCommand command;

            command.type = EyeControlCommand::ANIMATE;
            command.eyeId = atoi(eyeIdStr);
            command.animation_mode = atoi(animTypeStr);
            command.x = atoi(xStr);
            command.y = atoi(yStr);
            command.duration = atoi(durationStr);

            if (xQueueSend(eyeControlCommandQueue, &command, pdMS_TO_TICKS(10)) != pdPASS) {
                printf("Failed to send eye animate command\r\n");
            }

        } else {
            printf("Invalid arguments for E102 command\r\n");
        }

    }
/*=== DISPLAYS  ===*/
/*=== LEDSTRIP  === */
    static void interpretLEDStripSetColor(EmbeddedCli* cli, char* args, void* context) {
        uint16_t argCount = embeddedCliGetTokenCount(args);

        if (argCount == 3) {
            const char* rStr = embeddedCliGetToken(args, 1);
            const char* gStr = embeddedCliGetToken(args, 2);
            const char* bStr = embeddedCliGetToken(args, 3);
            LEDStripCommand command;

            command.type = LEDStripCommand::SET_COLOR;
            command.color = {static_cast<uint8_t>(atoi(rStr)), static_cast<uint8_t>(atoi(gStr)), static_cast<uint8_t>(atoi(bStr)), 0};

            if (xQueueSend(ledStripCommandQueue, &command, pdMS_TO_TICKS(10)) != pdPASS) {
                printf("Failed to send LED strip color command\r\n");
            }

        } else {
            printf("Invalid arguments for L101 command\r\n");
        }
    }

    static void interpretLEDStripSetBrightness(EmbeddedCli* cli, char* args, void* context) {
        uint16_t argCount = embeddedCliGetTokenCount(args);

        if (argCount == 1) {
            const char* brightnessStr = embeddedCliGetToken(args, 1);
            LEDStripCommand command;

            command.type = LEDStripCommand::SET_BRIGHTNESS;
            command.brightness = static_cast<uint8_t>(atoi(brightnessStr));

            if (xQueueSend(ledStripCommandQueue, &command, pdMS_TO_TICKS(10)) != pdPASS) {
                printf("Failed to send LED strip brightness command\r\n");
            }

        } else {
            printf("Invalid arguments for L102 command\r\n");
        }
    }

    static void interpretLEDStripFade(EmbeddedCli* cli, char* args, void* context) {
        uint16_t argCount = embeddedCliGetTokenCount(args);

        if (argCount == 4) {
            const char* rStr = embeddedCliGetToken(args, 1);
            const char* gStr = embeddedCliGetToken(args, 2);
            const char* bStr = embeddedCliGetToken(args, 3);
            const char* delayStr = embeddedCliGetToken(args, 4);
            LEDStripCommand command;

            command.type = LEDStripCommand::FADE;
            command.color = {static_cast<uint8_t>(atoi(rStr)), static_cast<uint8_t>(atoi(gStr)), static_cast<uint8_t>(atoi(bStr)), 0};
            command.fadeDelay = atof(delayStr);

            if (xQueueSend(ledStripCommandQueue, &command, pdMS_TO_TICKS(10)) != pdPASS) {
                printf("Failed to send LED strip fade command\r\n");
            }

        } else {
            printf("Invalid arguments for L103 command\r\n");
        }
    }
/*=== LEDSTRIP  === */
} // namespace COM
#endif // COMMAND_HANDLERS_HPP  