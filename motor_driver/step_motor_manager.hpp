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
