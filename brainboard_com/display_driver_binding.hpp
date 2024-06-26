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

#ifndef DISPLAY_DRIVER_BINDING_HPP
#define DISPLAY_DRIVER_BINDING_HPP

#include "embedded_cli.h"
#include "command_defines.hpp"

#include "FreeRTOS.h"
#include "queue.h"

namespace COM {

extern QueueHandle_t eyeControlCommandQueue;

/* 1 - COMMAND DEFINES */
enum class DisplayCommandType {
    MOVE_               = 0,
    BLINK_ANIM          = 1,
    CONFUSED_ANIM       = 2,
    THINKING_ANIM       = 3,
    DISABLE             = 4,
};

struct DisplayCommand {
    DisplayCommandType type;
    int displayId;
    int x_pos;
    int y_pos;
    int duration;

    union {
        int anim_type;
        bool disable;
    } data;
};

/* 2 - HANDLERS */
static void handleMove(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 4) {
        const char* displayIdStr = embeddedCliGetToken(args, 1);
        const char* xPosStr = embeddedCliGetToken(args, 2);
        const char* yPosStr = embeddedCliGetToken(args, 3);
        const char* durationStr = embeddedCliGetToken(args, 4);

        const int displayId = atoi(displayIdStr);
        const int xPos = atoi(xPosStr);
        const int yPos = atoi(yPosStr);
        const int duration = atoi(durationStr);

        if (displayId != -1 && xPos != -1 && yPos != -1 && duration != -1) {
            DisplayCommand command;
            command.type = DisplayCommandType::MOVE_;
            command.displayId = displayId;
            command.x_pos = xPos;
            command.y_pos = yPos;
            command.duration = duration;
            xQueueSend(eyeControlCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for DISPLAY MOVE command\n");
        }
    } else {
        printf("Invalid argument count for DISPLAY MOVE command\n");
    }
}

static void handleBlinkAnim(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 2) {
        const char* displayIdStr = embeddedCliGetToken(args, 1);
        const char* animTypeStr = embeddedCliGetToken(args, 2);

        const int displayId = atoi(displayIdStr);
        const int animType = atoi(animTypeStr);

        if (displayId != -1 && animType != -1) {
            DisplayCommand command;
            command.type = DisplayCommandType::BLINK_ANIM;
            command.displayId = displayId;
            command.data.anim_type = animType;
            xQueueSend(eyeControlCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for DISPLAY BLINK_ANIM command\n");
        }
    } else {
        printf("Invalid argument count for DISPLAY BLINK_ANIM command\n");
    }
}

static void handleConfusedAnim(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 2) {
        const char* displayIdStr = embeddedCliGetToken(args, 1);
        const char* animTypeStr = embeddedCliGetToken(args, 2);

        const int displayId = atoi(displayIdStr);
        const int animType = atoi(animTypeStr);

        if (displayId != -1 && animType != -1) {
            DisplayCommand command;
            command.type = DisplayCommandType::CONFUSED_ANIM;
            command.displayId = displayId;
            command.data.anim_type = animType;
            xQueueSend(eyeControlCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for DISPLAY CONFUSED_ANIM command\n");
        }
    } else {
        printf("Invalid argument count for DISPLAY CONFUSED_ANIM command\n");
    }
}

static void handleThinkingAnim(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 2) {
        const char* displayIdStr = embeddedCliGetToken(args, 1);
        const char* animTypeStr = embeddedCliGetToken(args, 2);

        const int displayId = atoi(displayIdStr);
        const int animType = atoi(animTypeStr);

        if (displayId != -1 && animType != -1) {
            DisplayCommand command;
            command.type = DisplayCommandType::THINKING_ANIM;
            command.displayId = displayId;
            command.data.anim_type = animType;
            xQueueSend(eyeControlCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for DISPLAY THINKING_ANIM command\n");
        }
    } else {
        printf("Invalid argument count for DISPLAY THINKING_ANIM command\n");
    }
}

static void handleDisable(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 1) {
        const char* displayIdStr = embeddedCliGetToken(args, 1);

        const int displayId = atoi(displayIdStr);

        if (displayId != -1) {
            DisplayCommand command;
            command.type = DisplayCommandType::DISABLE;
            command.displayId = displayId;
            command.data.disable = true;
            xQueueSend(eyeControlCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for DISPLAY DISABLE command\n");
        }
    } else {
        printf("Invalid argument count for DISPLAY DISABLE command\n");
    }
}

/* 3 - CLI BINDINGS */
static void addDisplayCliBindings(EmbeddedCli* cli) {
    addCliCommandBinding(cli, "DS101",
        "Move display: DISPLAY MOVE <DisplayID> <X_Pos> <Y_Pos> <Duration>", 
        true, nullptr, handleMove);

    addCliCommandBinding(cli, "DS102", 
        "Blink animation: DISPLAY BLINK_ANIM <DisplayID> <AnimType>", 
        true, nullptr, handleBlinkAnim);

    addCliCommandBinding(cli, "DS103", 
        "Confused animation: DISPLAY CONFUSED_ANIM <DisplayID> <AnimType>", 
        true, nullptr, handleConfusedAnim);

    addCliCommandBinding(cli, "DS104", 
        "Thinking animation: DISPLAY THINKING_ANIM <DisplayID> <AnimType>", 
        true, nullptr, handleThinkingAnim);

    addCliCommandBinding(cli, "DS105",
        "Disable display: DISPLAY DISABLE <DisplayID>", 
        true, nullptr, handleDisable);
}

} // namespace COM
#endif // DISPLAY_DRIVER_BINDING_HPP