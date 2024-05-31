#ifndef COMMAND_DEFINES_HPP
#define COMMAND_DEFINES_HPP

#include <PicoLed.hpp>

/* SIZE REFERENCES: */
#define MOTOR_COMMAND_SIZE sizeof(MotorCommand)
#define EYE_COMMAND_SIZE sizeof(EyeControlCommand)
#define LEDSTRIP_COMMAND_SIZE sizeof(LEDStripCommand)

#define ARGUMENT_COUNT 5
#define MOTOR_ARGUMENT_COUNT 4
#define EYE_ARGUMENT_COUNT 4
#define LEDSTRIP_ARGUMENT_COUNT 6

namespace COM {
/* 2 AXIS MOTOR - HEAD MOVEMENT */
struct MotorCommand {
    enum CommandType { 
        CONFIGURE       = 0, 
        CONTROL         = 1, 
    } type;

    int motorId;        // 0 for x-axis, 1 for y-axis;

    // CONFIGURATION COMMANDS: 
    int speed;          // Motor speed in RPM.  
    int yaw_angle;      // X-Horizontal movement setpoint.
    int pitch_angle;    // Y-Vertical movement setpoint.

    // CONTROL COMMANDS:
    bool start;        // Start or stop motor.
};

/* EYE DISPLAYS */
struct EyeControlCommand {
    enum CommandType {
        ANIMATE         = 0,
        MOVE            = 1,
        IDLE            = 2,
        RESET           = 3,
        DISABLE         = 4,
    } type;

    int eyeId;          // 0 for left eye, 1 for right eye, 2 for both eyes;
    int x;
    int y;
    int duration;

    // ANIMATION COMMANDS:
    int animation_mode; // Eye animation mode.
};

// For at the time being: 
struct LEDStripCommand {
    enum Type {
        SET_COLOR,
        SET_BRIGHTNESS,
        FADE
    } type;

    uint8_t brightness;
    PicoLed::Color color;
    double fadeDelay;
};

// struct LEDStripCommand {
//     enum CommandType { 
//         CONFIGURE       = 0, 
//         CONTROL         = 1, 
//         IDLE            = 2,
//         RESET           = 3,
//         DISABLE         = 4,
//     } type;

//     int ledID;          // 0 for LEDSTRP1, 1 for LEDSTRP2; 2 for both LEDSTRP1 and LEDSTRP2;

//     // CONFIGURATION COMMANDS: 
//     int brightness;     // LED brightness in percentage.
//     int effect_mode;    // LED mode.
//     int effect_delay;   // LED effect speed.
    
//     int color_r;        // Red    - LED color.
//     int color_g;        // Green  - LED color.
//     int color_b;        // Blue   - LED color.

//     // CONTROL COMMANDS:
//     bool start;         // Start or stop LED strip.
// };  

} // namespace COM
#endif // COMMAND_DEFINES_HPP