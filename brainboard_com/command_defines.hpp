#ifndef COMMAND_DEFINES_HPP
#define COMMAND_DEFINES_HPP

namespace COM {
struct MotorCommand {
    enum CommandType { 
        CONFIGURE, 
        CONTROL 
    } type;

    int motorId;

    // CONFIGURATION COMMANDS: 
    int speed;

    // CONTROL COMMANDS:
    bool start; 
};

// struct LEDStripCommand {
//     int ledStripId;
//     int mode;
//     int color;
// };  
} // namespace COM
#endif // COMMAND_DEFINES_HPP