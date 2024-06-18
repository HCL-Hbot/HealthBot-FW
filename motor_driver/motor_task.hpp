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

#ifndef MOTOR_TASK_HPP
#define MOTOR_TASK_HPP

#include "step_motor.hpp"
#include "step_motor_manager.hpp"
#include "motor_driver_binding.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

namespace MOTOR {

static void motorTask(void* pvParameters) {
    COM::MotorCommand command;

    while (true) {
        if (xQueueReceive(COM::motorCommandQueue, &command, portMAX_DELAY) == pdPASS) {
            MOTOR::StepMotorDriver* motor = MOTOR::motorManager.getMotor(command.motorId);
            if (motor) {
                switch (command.type) {
                    case COM::MotorCommandType::SET_SPEED:
                        printf("Setting speed for motor ID %d to %d RPM\n", command.motorId, command.data.speed);
                        motor->setSpeed_rpm(command.data.speed);
                        break;
                    case COM::MotorCommandType::SET_ROTATION_ANGLE:
                        printf("Setting rotation angle for motor ID %d to %d degrees\n", command.motorId, command.data.angle);
                        motor->rotateDegree(static_cast<uint16_t>(command.data.angle));
                        break;
                    case COM::MotorCommandType::SET_DIRECTION:
                        printf("Setting direction for motor ID %d to %d\n", command.motorId, command.data.direction);
                        motor->setDirection(static_cast<MotorDirection_t>(command.data.direction));
                        break;
                    case COM::MotorCommandType::EXECUTE:
                        if (command.data.execute) {
                            printf("Executing start command for motor ID %d\n", command.motorId);
                            motor->enable();
                        } else {
                            printf("Executing stop command for motor ID %d\n", command.motorId);
                            motor->disable();
                        }
                        break;
                    case COM::MotorCommandType::SELECT:
                        printf("Selecting motor ID %d\n", command.motorId);
                        break;
                    default:
                        printf("Unknown command type for motor ID %d\n", command.motorId);
                        break;
                }
            } else {
                printf("Motor with ID %d not found\n", command.motorId);
            }
        }
    }
}

void startMotorTask() {
    xTaskCreate(motorTask, "MotorTask", 700, nullptr, 1, nullptr);
}

} // namespace MOTOR
#endif // MOTOR_TASK_HPP