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