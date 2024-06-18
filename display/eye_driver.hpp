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

#ifndef EYE_DISPLAY_DRIVER_HPP
#define EYE_DISPLAY_DRIVER_HPP

#include <memory>
#include <lv_drv_conf.h>
#include <lvgl.h>
#include "lv_port_disp.h"
#include "ui.h"
#include "ui_helpers.h"
#include "lvgl.h"

#include "display_driver_binding.hpp"
#include "eye_component.hpp"
#include <command_defines.hpp>

namespace DISPLAY {

class EyeDisplayDriver {
public:
    EyeDisplayDriver(lv_disp_t* disp1, lv_disp_t* disp2) {
        if (disp1) {
            lv_disp_set_default(disp1);
            eyeLeft = std::make_unique<EyeComponent>(lv_scr_act(), &ui_img_oog_gekozen_8_gesplitst_png, nullptr, nullptr);
        }
        if (disp2) {
            lv_disp_set_default(disp2);
            eyeRight = std::make_unique<EyeComponent>(lv_scr_act(), &ui_img_oog_gekozen_8_gesplitst_png, nullptr, nullptr);
        }
    }

    ~EyeDisplayDriver() {}

    void startTasks() {
        xTaskCreate(EyeDisplayDriver::displayHandler, "EyeDisplayHandler", 1500, this, 2, nullptr);
        xTaskCreate(EyeDisplayDriver::runCommandHandle, "EyeControlHandle", 1200, this, 1, nullptr);
    }

    static void displayHandler(void *pvParameters) {

        while (true) {
            lv_tick_inc(1);
            vTaskDelay(1 / portTICK_PERIOD_MS);
            lv_task_handler();
        }
    }

    static void runCommandHandle(void *pvParameters) {
        auto *driver = static_cast<EyeDisplayDriver*>(pvParameters);
        COM::DisplayCommand command;

        while (true) {
            if (xQueueReceive(COM::eyeControlCommandQueue, &command, portMAX_DELAY) == pdPASS) {
                driver->handleEyeCommand(command);
            }
        }
    }

private:
    std::unique_ptr<EyeComponent> eyeLeft;
    std::unique_ptr<EyeComponent> eyeRight;

    void handleEyeCommand(const COM::DisplayCommand& command) {
        switch (command.type) {
            case COM::DisplayCommandType::MOVE_:
                if (command.displayId == 0 || command.displayId == 2) {
                    eyeLeft->animate_to_xy(eyeLeft->getPupil(), command.x_pos, command.y_pos, command.duration);
                }
                if (command.displayId == 1 || command.displayId == 2) {
                    eyeRight->animate_to_xy(eyeRight->getPupil(), command.x_pos, command.y_pos, command.duration);
                }
                break;
            case COM::DisplayCommandType::BLINK_ANIM:
                if (command.displayId == 0 || command.displayId == 2) {
                    eyeLeft->animate_blink();
                }
                if (command.displayId == 1 || command.displayId == 2) {
                    eyeRight->animate_blink();
                }
                break;
            case COM::DisplayCommandType::CONFUSED_ANIM:
                if (command.displayId == 0 || command.displayId == 2) {
                    eyeLeft->animate_confused();
                }
                if (command.displayId == 1 || command.displayId == 2) {
                    eyeRight->animate_confused();
                }
                break;
            case COM::DisplayCommandType::THINKING_ANIM:
                if (command.displayId == 0 || command.displayId == 2) {
                    eyeLeft->animate_thinking();
                }
                if (command.displayId == 1 || command.displayId == 2) {
                    eyeRight->animate_thinking();
                }
                break;
            case COM::DisplayCommandType::DISABLE:
                if (command.displayId == 0 || command.displayId == 2) {
                    eyeLeft->disable();
                }
                if (command.displayId == 1 || command.displayId == 2) {
                    eyeRight->disable();
                }
                break;
        }
    }
}; // class EyeDisplayDriver
} // namespace DISPLAY
#endif // EYE_DISPLAY_DRIVER_HPP