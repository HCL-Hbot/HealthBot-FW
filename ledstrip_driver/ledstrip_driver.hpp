#ifndef LEDSTRIP_DRIVER_HPP
#define LEDSTRIP_DRIVER_HPP

#include <PicoLed.hpp>
#include <Effects/Fade.hpp>
#include <memory>
#include <map>

namespace LED {

class LedStripDriver {
public:
    LedStripDriver(PIO pio, uint8_t sm, uint8_t pin, uint16_t numLeds, PicoLed::DataFormat format)
        : strip(PicoLed::addLeds<PicoLed::WS2812B>(pio, sm, pin, numLeds, format)) {
        turn_off();
    }

    void turn_off() {
        strip.clear();
        show();
    }

    void set_brightness(const uint8_t brightness) {
        strip.setBrightness(brightness);
    }

    void set_color(const PicoLed::Color color) {
        strip.fill(color);
        show();
    }

    void show() {
        strip.show();
    }

    void effect_rainbow(const uint16_t numLeds) {
        strip.fillRainbow(0, 255 / numLeds);
        show();
    }

    void effect_fade_in(const PicoLed::Color color, const uint32_t fade_interval_ms) {
        for (int i = 0; i < 255; i++) {
            set_color(PicoLed::RGB(color.red * i / 255, color.green * i / 255, color.blue * i / 255));
            show();
            vTaskDelay(pdMS_TO_TICKS(fade_interval_ms));
        }
    }

    void effect_fade_out(const PicoLed::Color color, const uint32_t fade_interval_ms) {
        for (int i = 255; i > 0; i--) {
            set_color(PicoLed::RGB(color.red * i / 255, color.green * i / 255, color.blue * i / 255));
            show();
            vTaskDelay(pdMS_TO_TICKS(fade_interval_ms));
        }
    }

    void effect_fade_inout(const PicoLed::Color color, const uint32_t keep_on_interval_ms, int fade_interval_ms) {
        effect_fade_in(color, fade_interval_ms);
        vTaskDelay(pdMS_TO_TICKS(keep_on_interval_ms));
        effect_fade_out(color, fade_interval_ms);
    }

private:
    PicoLed::PicoLedController strip;

}; // class LedStripDriver
} // namespace LED
#endif // LEDSTRIP_DRIVER_HPP