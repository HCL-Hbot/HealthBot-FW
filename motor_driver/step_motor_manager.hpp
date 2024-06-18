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

#ifndef STEP_MOTOR_MANAGER_HPP
#define STEP_MOTOR_MANAGER_HPP

#include "step_motor.hpp"

namespace MOTOR {

class MotorManager {
public:
    void addMotor(int motorId, std::unique_ptr<MOTOR::StepMotorDriver> motor) {
        motors[motorId] = std::move(motor);
    }

    MOTOR::StepMotorDriver* getMotor(int motorId) {
        auto it = motors.find(motorId);
        if (it != motors.end()) {
            return it->second.get();
        } else {
            return nullptr;
        }
    }

private:
    std::map<int, std::unique_ptr<MOTOR::StepMotorDriver>> motors;
};

MotorManager motorManager;

} // namespace MOTOR
#endif // STEP_MOTOR_MANAGER_HPP
