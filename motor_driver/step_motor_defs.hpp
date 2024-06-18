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

#ifndef STEP_MOTOR_DEFS_HPP
#define STEP_MOTOR_DEFS_HPP

#include <stdint.h>

#define HIGH    1
#define LOW     0
#define PI      (3.14159f)

namespace MOTOR {

typedef enum class RotationSpeedDefaults: uint8_t {
    RPM1   = 1,
    RPM5   = 5,
    RPM10  = 10,
    RPM20  = 20,
    RPM30  = 30,
    RPM40  = 40,
    RPM50  = 50,
    RPM60  = 60,
} RotationSpeedDefaults_t;

typedef enum class MotorDirection : uint8_t {
    MOTOR_DIRECTION_CW = 0,
    MOTOR_DIRECTION_CCW = 1
} MotorDirection_t;

typedef enum class DefaultAngles : uint16_t {
    ANGLE_0     = 0,
    ANGLE_30    = 30,
    ANGLE_60    = 60,
    ANGLE_90    = 90,
    ANGLE_120   = 120,
    ANGLE_150   = 150,
    ANGLE_180   = 180,
    ANGLE_210   = 210,
    ANGLE_240   = 240,
    ANGLE_270   = 270,
    ANGLE_300   = 300,
    ANGLE_330   = 330,
    ANGLE_360   = 360,

    ANGLE_5     = 5,   
    ANGLE_10    = 10,
    ANGLE_20    = 20,
} DefaultAngles_t;

};

#endif // STEP_MOTOR_DEFS_HPP