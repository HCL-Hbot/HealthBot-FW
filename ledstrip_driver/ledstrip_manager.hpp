#ifndef LEDSTRIP_MANAGER_HPP
#define LEDSTRIP_MANAGER_HPP

#include <PicoLed.hpp>
#include <Effects/Fade.hpp>
#include <memory>
#include <map>

#include "ledstrip_driver_binding.hpp"

namespace LED {

class LedStripManager {
public:
    void addLedStrip(const uint8_t strip_id, std::unique_ptr<LedStripDriver> strip) {
        strips[strip_id] = std::move(strip);
    }

    LedStripDriver* getLedStrip(int stripId) {
        auto it = strips.find(stripId);
        if (it != strips.end()) {
            return it->second.get();
        } else {
            return nullptr;
        }
    }

    void runCommandHandle() {
        COM::LedstripCommand command;

        while (true) {
            if (xQueueReceive(COM::ledStripCommandQueue, &command, portMAX_DELAY) == pdPASS) {
                LedStripDriver* strip = getLedStrip(command.ledstripId);
                if (strip) {
                    switch (command.type) {
                        case COM::LedstripCommandType::SET_RGB:
                            strip->set_color(PicoLed::RGB(command.data.rgb.r, command.data.rgb.g, command.data.rgb.b));
                            strip->show();
                            break;
                        case COM::LedstripCommandType::ANIM_BLINK:
                            strip->set_color(PicoLed::RGB(command.data.anim_blink.r, command.data.anim_blink.g, command.data.anim_blink.b));
                            strip->show();
                            vTaskDelay(pdMS_TO_TICKS(command.data.anim_blink.duration));
                            strip->set_color(PicoLed::RGB(0, 0, 0));
                            strip->show();
                            break;
                        case COM::LedstripCommandType::ANIM_FADE_OUT:
                            strip->effect_fade_out(PicoLed::RGB(255, 255, 255), command.data.anim_fade_out.duration);
                            break;
                        case COM::LedstripCommandType::ANIM_FADE_IN:
                            strip->effect_fade_in(PicoLed::RGB(255, 255, 255), command.data.anim_fade_in.duration);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    void start() {
        xTaskCreate([](void* param) {
            static_cast<LedStripManager*>(param)->runCommandHandle();
        }, "LedstripCommandHandle", 1200, this, 1, nullptr);
    }

private:
    std::map<uint8_t, std::unique_ptr<LedStripDriver>> strips;
};
} // namespace LED
#endif // LEDSTRIP_MANAGER_HPP