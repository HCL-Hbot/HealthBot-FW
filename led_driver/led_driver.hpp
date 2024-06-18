#ifndef LED_DRIVER_HPP
#define LED_DRIVER_HPP

#include <pico/stdlib.h>
#include <expander_driver.hpp>

namespace LED {

class LedDriver {
public:
    LedDriver(EXPANDER::TCA9534XXX &expander, const uint8_t pin) 
        : expander_(expander), pin_(pin) {
        expander_.setPinDirection(pin_, EXPANDER::DIRECTION::OUTPUT);
        expander_.setPin(pin_, EXPANDER::LEVEL::LOW);
    }

    void turn_on() {
        expander_.setPin(pin_, EXPANDER::LEVEL::HIGH);
    }

    void turn_off() {
        expander_.setPin(pin_, EXPANDER::LEVEL::LOW);
    }

    void runCommandHandle() {
        COM::LedDriverCommand command;

        while (true) {
            if (xQueueReceive(COM::leddriver_command_queue, &command, portMAX_DELAY) == pdPASS) {
                switch (command.type) {
                    case COM::LedCommandType::SET_LED:
                        if (command.turnOn == 1) {
                            turn_on();
                        } else {
                            turn_off();
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    static void start(void* pvParameters) {
        LedDriver* ledDriver = static_cast<LedDriver*>(pvParameters);
        xTaskCreate([](void* pvParameters) {
            static_cast<LedDriver*>(pvParameters)->runCommandHandle();
        }, "LedCommandHandle", 400, ledDriver, 1, nullptr);
    }

private:
    EXPANDER::TCA9534XXX &expander_;
    const uint8_t pin_;
}; // class LedDriver
} // namespace LED
#endif // LED_DRIVER_HPP