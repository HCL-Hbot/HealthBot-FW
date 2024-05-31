#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <PicoLed.hpp>
#include <Effects/Fade.hpp>

namespace LED
{
    class LedStripController
    {
    public:
        LedStripController(uint pin1, uint numLeds1, uint pin2, uint numLeds2)
            : strip1(PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, pin1, numLeds1, PicoLed::FORMAT_GRB)),
              strip2(PicoLed::addLeds<PicoLed::WS2812B>(pio1, 0, pin2, numLeds2, PicoLed::FORMAT_GRB))
        {
            clear();
        }

        void clear()
        {
            strip1.clear();
            strip2.clear();
            show();
        }

        void setBrightness(uint8_t brightness)
        {
            strip1.setBrightness(brightness); // Max brightness
            strip2.setBrightness(brightness);
        }
        void fill(PicoLed::Color color)
        {
            strip1.fill(color);
            strip2.fill(color);
        }

        void show()
        {
            strip1.show();
            strip2.show();
        }

        void fillRainbow(uint numLeds1, uint numLeds2)
        {
            strip1.fillRainbow(0, 255 / numLeds1);
            strip2.fillRainbow(0, 255 / numLeds2);
        }

        void fade(PicoLed::Color color, double factor){
            strip1.fade(color, factor);
            strip2.fade(color, factor);
        }

    void effectFade( PicoLed::Color color, double fadeDelay){
        PicoLed::Fade effectFade(strip2, color, fadeDelay);
        effectFade.animate();
        effectFade.reset();
        //PicoLed::Fade effectFade(strip2, color, fadeRate);
    }

    private:
        PicoLed::PicoLedController strip1;
        PicoLed::PicoLedController strip2;

    }; // class LedStripController
} // namespace LED
#endif // LEDSTRIP_H