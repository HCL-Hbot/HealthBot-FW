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

#ifndef MOTOR_DRIVER_BINDING_HPP
#define MOTOR_DRIVER_BINDING_HPP

#include "embedded_cli.h"
#include "command_defines.hpp"

#include "FreeRTOS.h"
#include "queue.h"

namespace COM {

extern QueueHandle_t motorCommandQueue;

/* 1 - COMMAND DEFINES */
enum class MotorCommandType {
    SET_SPEED           = 0,
    SET_ROTATION_ANGLE  = 1,
    SET_DIRECTION       = 2,
    EXECUTE             = 3,
    SELECT              = 4,
};

struct MotorCommand {
    MotorCommandType type;
    int motorId;

    union {
        int speed;
        int angle;
        int direction;
        bool execute;
    } data;
};

/* 2 - HANDLERS */
static void handleSetSpeed(EmbeddedCli* cli, char* args, void* context) {
    uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 2) {
        const char* motorIdStr = embeddedCliGetToken(args, 1);
        const char* speedStr = embeddedCliGetToken(args, 2);

        const int motorId = atoi(motorIdStr);
        const int speed = atoi(speedStr);

        if (motorId != -1 && speed != -1) {
            MotorCommand command;
            command.type = MotorCommandType::SET_SPEED;
            command.motorId = motorId;
            command.data.speed = speed;
            xQueueSend(motorCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for MOTOR SET_SPEED command\n");
        }
        
    } else {
        printf("Invalid argument count for MOTOR SET_SPEED command\n");
    }
}

static void handleSetRotationAngle(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 2) {
        const char* motorIdStr = embeddedCliGetToken(args, 1);
        const char* angleStr = embeddedCliGetToken(args, 2);

        const int motorId = atoi(motorIdStr);
        const int angle = atoi(angleStr);

        if (motorId != -1 && angle != -1) {
            MotorCommand command;
            command.type = MotorCommandType::SET_ROTATION_ANGLE;
            command.motorId = motorId;
            command.data.angle = angle;
            xQueueSend(motorCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for MOTOR SET_ROTATION_ANGLE command\n");
        }
    } else {
        printf("Invalid argument count for MOTOR SET_ROTATION_ANGLE command\n");
    }
}

static void handleSetDirection(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 2) {
        const char* motorIdStr = embeddedCliGetToken(args, 1);
        const char* directionStr = embeddedCliGetToken(args, 2);

        const int motorId = atoi(motorIdStr);
        const int direction = atoi(directionStr);

        if (motorId != -1 && direction != -1) {
            MotorCommand command;
            command.type = MotorCommandType::SET_DIRECTION;
            command.motorId = motorId;
            command.data.direction = direction;
            xQueueSend(motorCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for MOTOR SET_DIRECTION command\n");
        }
    } else {
        printf("Invalid argument count for MOTOR SET_DIRECTION command\n");
    }
}

static void handleExecute(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 2) {
        const char* motorIdStr = embeddedCliGetToken(args, 1);
        const char* actionStr = embeddedCliGetToken(args, 2);

        const int motorId = atoi(motorIdStr);
        const bool execute = (strcmp(actionStr, "start") == 0);

        if (motorId != -1) {
            MotorCommand command;
            command.type = MotorCommandType::EXECUTE;
            command.motorId = motorId;
            command.data.execute = execute;
            xQueueSend(motorCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for MOTOR EXECUTE command\n");
        }
    } else {
        printf("Invalid argument count for MOTOR EXECUTE command\n");
    }
}

static void handleSelect(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 1) {
        const char* motorIdStr = embeddedCliGetToken(args, 1);

        const int motorId = atoi(motorIdStr);

        if (motorId != -1) {
            MotorCommand command;
            command.type = MotorCommandType::SELECT;
            command.motorId = motorId;
            xQueueSend(motorCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for MOTOR SELECT command\n");
        }
    } else {
        printf("Invalid argument count for MOTOR SELECT command\n");
    }
}

/* 3 - CLI BINDINGS */
static void addMotorCliBindings(EmbeddedCli* cli) {
    addCliCommandBinding(cli, "M101",
        "Set motor speed: MOTOR SET_SPEED <MotorID> <Speed>", 
        true, nullptr, handleSetSpeed);

    addCliCommandBinding(cli, "M102", 
        "Set motor rotation angle: MOTOR SET_ROTATION_ANGLE <MotorID> <Angle>", 
        true, nullptr, handleSetRotationAngle);

    addCliCommandBinding(cli, "M103", 
        "Set motor direction: MOTOR SET_DIRECTION <MotorID> <Direction>", 
        true, nullptr, handleSetDirection);

    addCliCommandBinding(cli, "M104", 
        "Execute motor command: MOTOR EXECUTE <MotorID> <start|stop>", 
        true, nullptr, handleExecute);
        
    addCliCommandBinding(cli, "M105",
        "Select motor reference: MOTOR SELECT <MotorID>", 
        true, nullptr, handleSelect);
}

} // namespace COM
#endif // MOTOR_DRIVER_BINDING_HPP