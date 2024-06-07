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