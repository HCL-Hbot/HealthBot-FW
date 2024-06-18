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

#ifndef STEP_MOTOR_HPP
#define STEP_MOTOR_HPP 

#include <math.h> // For Round function.
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <step_motor_defs.hpp>

/* To do list
 - Add error handling.
 - Add boundary condition checking.
 - Add hold the motor at the end of the rotation option.
 - Add acceleration and deceleration functionality. 
 - Set zero position for homing.
 - Add UART configuration of the driver.
 - States instead of the two bools.
*/

namespace MOTOR {
class StepMotorDriver {  
public:
    StepMotorDriver(const uint8_t step_pin, const uint8_t dir_pin, const uint8_t en, const MotorDirection motor_direction = MotorDirection::MOTOR_DIRECTION_CCW) 
        : step_pin(step_pin), dir_pin(dir_pin), en_pin(en), motor_direction(motor_direction) 
    {
        current_step_counter = 0;
        is_initialized = false;
        gpio_init(step_pin);
        gpio_set_dir(step_pin, GPIO_OUT);
        gpio_init(dir_pin);
        gpio_set_dir(dir_pin, GPIO_OUT);
        gpio_init(en_pin);
        gpio_set_dir(en_pin, GPIO_OUT);

        printf("Motor initialized: step_pin=%d, dir_pin=%d, en_pin=%d\n", step_pin, dir_pin, en_pin);
        initPulseGenerator(0, 100, 1000); // Default values.
    }
    
    ~StepMotorDriver() {
        disable();
    }

    void initPulseGenerator(const uint32_t pulses, int64_t width_us, int64_t period_between_us) {

        if (width_us >= period_between_us) {
            return; // Error handling. 
        }

        // Clamp to Minimal pulse width:
        if (width_us < min_pulse_width_us) {
            width_us = min_pulse_width_us;
        } 

        // Clamp to Minimal period:
        if (period_between_us < min_period_between_us) {
            period_between_us = min_period_between_us;
        }

        current_step_counter = 0;
        current_speed_deg_per_sec = calculateSpeed(pulses, period_between_us);
        current_pulse_width_us = width_us;
        current_period_between_us = period_between_us;
        target_step_counter = pulses;
        is_initialized = true;
    }

    // Nice to have a error handling here. 
    void enable() {
        if (! is_initialized) {
            return;
        } else if (is_enabled) {
            is_enabled = false;
            current_step_counter = 0;
        }

        gpio_put(en_pin, LOW); // Enable the motor driver.
        gpio_put(dir_pin, static_cast<bool>(motor_direction));
        add_repeating_timer_us(-current_period_between_us, start_pulse_callback, this, &start_pulse_timer);
    }

    void reset() {
        current_step_counter = 0;
        add_repeating_timer_us(-current_period_between_us, start_pulse_callback, this, &start_pulse_timer);
    }

    void disable() {
        is_initialized = false;
        gpio_put(step_pin, LOW);
        gpio_put(dir_pin, LOW);
        gpio_put(en_pin, HIGH);
    }

    // ATTENTION: TOTALLTY NOT PROPERLY YET. 
    // There should be a throughful boundary condition checking. And error handling 
    void setSpeed_dps(const float deg_per_sec) {
        if (deg_per_sec > 0) {
            int64_t pulses = static_cast<int64_t>((deg_per_sec/step_deg_resolution));
            int64_t new_period_between_us = (1.0f*1e6/pulses); // in 1 sec I want to generate so many pulses.
            initPulseGenerator(pulses, current_pulse_width_us, new_period_between_us);
        }
    }

    void setSpeed_rpm(const uint32_t rpm) {
        if (rpm > 0) {
            const float dps = convertRPMtoDegPerSec(rpm);
            int64_t pulses = static_cast<int64_t>((dps/step_deg_resolution));
            int64_t new_period_between_us = (1.0f*1e6/pulses); // in 1 sec I want to generate so many pulses.
            initPulseGenerator(pulses, current_pulse_width_us, new_period_between_us);
        }
    }
    
    // Deg per sec.
    const float getSpeed_dps() {
        return current_speed_deg_per_sec;
    }
    
    // Rotations per minute.
    const uint32_t getSpeed_rpm() {
        return static_cast<uint32_t>(round((current_speed_deg_per_sec*60.0f)/360.0f));
    }

    void setDirection(const MotorDirection_t direction) {
        motor_direction = direction;
    }

    const MotorDirection_t getDirection() {
        return motor_direction;
    }

    void rotateDegree(const uint16_t degree) {
        const uint32_t steps = steps_per_deg * degree;
        initPulseGenerator(steps, current_pulse_width_us, current_period_between_us);
        enable();
    }

    void rotateDegree(const float degree) {
        if (degree < 0) {
            return; // For now just return, later direction could be reversed.
        }
        float wrapped_degree = fmod(degree, 360.0f);
        const uint32_t steps = static_cast<uint32_t>(std::round(steps_per_deg * wrapped_degree));
        initPulseGenerator(steps, current_pulse_width_us, current_period_between_us);
        enable();
    }

    void rotateRadians(float radian) {
        if (radian < 0) {
            return; // For now just return, later direction could be reversed.
        }
        float rotation_in_deg = radian * (180.0f/PI);
        rotateDegree(rotation_in_deg);
    }

    void rotateAngle(DefaultAngles_t angle) {
        rotateDegree(static_cast<uint16_t>(angle));
    }

    void rotateSteps(const uint32_t step) {
        initPulseGenerator(step, current_pulse_width_us, current_period_between_us);
        enable();
    }

private:   
    const static inline uint16_t motor_steps_per_revolution = 200;      // NEMA17 1.8° per step.
    const static inline uint16_t microstep_resolution       = 16;       // TMC2209 has integrated interpolation to output 256. 
    const constexpr static inline uint32_t microsteps_per_revolution = (motor_steps_per_revolution * microstep_resolution); // 51200 microsteps/rev.
    const constexpr static inline float step_deg_resolution = (360.0f / microsteps_per_revolution);                     // 0.00703125° per step.
    const constexpr static inline float steps_per_deg       = (1.0f / step_deg_resolution); // 142.857 steps/1.0°.
    
    // From desk lab experiment: according to the datasheet the minimal pulse width is 20us, 
    // but this is not recommended because some steps will be skipped 
    // (althoug through measurements the step width is consistently 20us)
    const static inline int16_t min_pulse_width_us      = 30;
    const static inline int16_t min_period_between_us   = 100;

    const uint8_t step_pin;
    const uint8_t dir_pin;
    const uint8_t en_pin;

    bool is_initialized;
    bool is_enabled;

    repeating_timer_t start_pulse_timer;
    repeating_timer_t end_pulse_timer;

    uint32_t current_step_counter;
    uint32_t target_step_counter;
    int64_t current_pulse_width_us;
    int64_t current_period_between_us;
    MotorDirection_t motor_direction; 
    float current_speed_deg_per_sec;

    static bool start_pulse_callback(repeating_timer_t *rt) {
        StepMotorDriver *controller = reinterpret_cast<StepMotorDriver*>(rt->user_data); 
        controller->is_enabled = true;

        if (controller->current_step_counter >= controller->target_step_counter) {
            controller->disable();
            controller->is_enabled = false;
            return false; // Target Value reached.
        }

        gpio_put(controller->step_pin, HIGH); // Pulse Start.
        controller->current_step_counter++;

        // Schedule the end of the pulse:
        add_repeating_timer_us(-controller->current_pulse_width_us, 
            end_pulse_callback, controller, &controller->end_pulse_timer);
        return true;
    }

    static bool end_pulse_callback(repeating_timer_t *rt) {
        StepMotorDriver *controller = reinterpret_cast<StepMotorDriver*>(rt->user_data);
        gpio_put(controller->step_pin, LOW);
        return false;
    }

    constexpr static inline float convertRPMtoDegPerSec(const uint32_t rpm) {
        return (rpm*360.0f)/60.0f; 
    }

    constexpr static inline float calculateSpeed(const uint32_t pulses, const int64_t period_between_us) {
        return (static_cast<float>(pulses)*step_deg_resolution)/((period_between_us/1000'000.0f)*static_cast<float>(pulses));
    }
};
} // namespace MOTOR

#endif // STEP_MOTOR_HPP