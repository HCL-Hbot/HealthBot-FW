#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <PicoLed.hpp>
#include <Effects/Fade.hpp>

namespace LED
{
    class LedStripController
    {
    public:
        LedStripController(uint pin1, uint numLeds1, PicoLed::DataFormat format1, uint pin2, uint numLeds2, PicoLed::DataFormat format2)
            : strip1(PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, pin1, numLeds1, format1)),
              strip2(PicoLed::addLeds<PicoLed::WS2812B>(pio1, 0, pin2, numLeds2, format2))
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
            show();
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
            show();
        }

        void fadeIn(PicoLed::Color color, uint fadeIntervalTime)
        {
            for (int i = 0; i < 255; i++)
            {
                fill(PicoLed::RGB(color.red * i / 255, color.green * i / 255, color.blue * i / 255)); // Adjust color
                show();
                vTaskDelay(pdMS_TO_TICKS(fadeIntervalTime));
            }
        }

        void fadeOut(PicoLed::Color color, uint fadeIntervalTime)
        {
            for (int i = 255; i > 0; i--)
            {
                fill(PicoLed::RGB(color.red * i / 255, color.green * i / 255, color.blue * i / 255));
                show();
                vTaskDelay(pdMS_TO_TICKS(fadeIntervalTime));
            }
        }

        void fadeInOut(PicoLed::Color color, uint ledsOnTime, uint fadeIntervalTime)
        {
            fadeIn(color, fadeIntervalTime);
            vTaskDelay(pdMS_TO_TICKS(ledsOnTime));
            fadeOut(color, fadeIntervalTime);
        }

        void fade(PicoLed::Color color, double factor)
        {
            strip1.fade(color, factor);
            strip2.fade(color, factor);
            show();
        }

        void fadeValue(PicoLed::Color color, uint value)
        {
            strip1.fadeValue(color, value);
            strip2.fadeValue(color, value);
            show();
        }

        void effectFade(PicoLed::Color color, double fadeRate)
        {

            bool effectReset = false;
            PicoLed::Fade fadeEffect1(strip1, color, fadeRate);
            PicoLed::Fade fadeEffect2(strip2, color, fadeRate);

            if (effectReset)
            {
                effectReset = false;
                fadeEffect1.reset();
            }
            if (fadeEffect1.animate())
            {
                strip1.show();
            }
            if (effectReset)
            {
                effectReset = false;
                fadeEffect2.reset();
            }
            if (fadeEffect2.animate())
            {
                strip2.show();
            }

            // fadeEffect1.animate();
            // fadeEffect2.animate();
            // show();
        }

    private:
        PicoLed::PicoLedController strip1;
        PicoLed::PicoLedController strip2;

    }; // class LedStripController
} // namespace LED
#endif // LEDSTRIP_H
