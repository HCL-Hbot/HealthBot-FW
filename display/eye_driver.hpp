#ifndef EYE_DISPLAY_DRIVER_HPP
#define EYE_DISPLAY_DRIVER_HPP

#include <memory>
#include <lv_drv_conf.h>
#include <lvgl.h>
#include "lv_port_disp.h"
#include "ui.h"
#include "ui_helpers.h"
#include "lvgl.h"

#include "eye_component.hpp"

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

    ~EyeDisplayDriver() {
    }

    static void displayHandler(void *pvParameters) {
        auto *driver = static_cast<EyeDisplayDriver*>(pvParameters);
        lv_init();
        lv_port_disp_init();

        while (1) {
            lv_tick_inc(1);
            vTaskDelay(1 / portTICK_PERIOD_MS);
            lv_task_handler();
        }
        delete driver;
    }

    static void runEyeControlHandle(void *pvParameters) {
        auto *driver = static_cast<EyeDisplayDriver*>(pvParameters);
        driver->eyeMovement();
    }

private:
    std::unique_ptr<EyeComponent> eyeLeft;
    std::unique_ptr<EyeComponent> eyeRight;

   void eyeMovement() {
        uint8_t state = 0;
        while (true) {
            switch (state) {
                case 0: 
                    eyeLeft->animate_to_xy(eyeLeft->getPupil(), 27, 27, 150);
                    eyeRight->animate_to_xy(eyeRight->getPupil(), 27, 27, 200);
                
                    eyeRight->animate_to_xy(eyeRight->getThinking(), 27, 27, 200);
                    eyeRight->animate_thinking();

                    // eyeLeft->animate_confused();
                    state = 1;
                    break;
                case 1:
                    // eyeRight->animate_confused();
                    eyeLeft->animate_to_xy(eyeLeft->getPupil(), 27, -27, 150);
                    eyeRight->animate_to_xy(eyeRight->getPupil(), 27, -27, 200);
                    
                    eyeRight->animate_thinking();
                    eyeRight->animate_to_xy(eyeRight->getThinking(), 27, -27, 200);
                    //eyeRight->animate_thinking();

                    state = 2;
                    break;
                case 2:
                    eyeLeft->animate_to_xy(eyeLeft->getPupil(), -27, -27, 150);
                    eyeRight->animate_to_xy(eyeRight->getPupil(), -27, -27, 200);

                    eyeRight->animate_thinking();
                    eyeRight->animate_to_xy(eyeRight->getThinking(), -27, -27, 200);
                    //eyeRight->animate_thinking();

                    // eyeLeft->animate_confused();
                    // eyeLeft->animate_blink();
                    state = 3;
                    break;
                case 3:
                    // eyeRight->animate_confused();
                    // eyeLeft->animate_blink();

                    eyeLeft->animate_to_xy(eyeLeft->getPupil(), -27, 27, 150);
                    eyeRight->animate_to_xy(eyeRight->getPupil(), -27, 27, 200);

                    eyeRight->animate_thinking();
                    eyeRight->animate_to_xy(eyeRight->getThinking(), -27, 27, 200);
                    //eyeRight->animate_thinking();

                    state = 0;
                    break;
            }
            vTaskDelay(3000 / portTICK_PERIOD_MS);
        }
    }
};

} // namespace DISPLAY
#endif // EYE_DISPLAY_DRIVER_HPP


