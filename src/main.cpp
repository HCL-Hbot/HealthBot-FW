#include "hardware/structs/rosc.h"
#include <stdio.h>

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include <task.h>
#include "board_defines.h"
#include <hardware/spi.h>
#include <lv_drv_conf.h>
#include <lvgl.h>
#include "lv_port_disp.h"
#include "ui.h"
#include "tusb.h"

#include <eye_driver.hpp>
#include <UART_streamer.hpp>
#include "cli_port.hpp"
#include <ledstrip_driver.hpp>
#include <Effects/Fade.hpp>
#include "PicoLedEffect.hpp"
#include <expander_driver.hpp> 



enum ColorFormat
{
    RGB = PicoLed::FORMAT_RGB,
    GRB = PicoLed::FORMAT_GRB,
    WRGB = PicoLed::FORMAT_WRGB
};


/* Defines for ledstrip, maybe move to seperate file*/
#define NUM_LEDS1 30
#define NUM_LEDS2 60

#define FORMAT_LEDS1 GRB
#define FORMAT_LEDS2 GRB

#define LED_BRIGHTNESS 100

#define ledsOnTime 5000     // Milliseconds to keep the leds ON
#define ledsOffTime 5000    // Milliseconds to keep the leds OFF
#define fadeIntervalTime 15 // Time in between .show() in fade functions, recommended to not make it significantly higher
/*-------------------------------------------------------------*/


/* Defines for I/O Expander, maybe move to seperate file*/
#define TCA9534_ADDR 0x20 // Define the I2C address for the TCA9534

// Define the I2C instance and pins
#define I2C_PORT i2c1
#define I2C_SDA_PIN 26
#define I2C_SCL_PIN 27
#define I2C_BAUDRATE 100000
/*--------------------------------------------------------------*/


static inline SERIAL::UART_RTOS_Driver uartStreamer{UART_BAUD_RATE, UART_TX_PIN, UART_RX_PIN};
static inline LED::LedStripController ledStrips(
    LEDSTRIP1_DATAPIN,
    NUM_LEDS1,
    static_cast<PicoLed::DataFormat>(FORMAT_LEDS1),
    LEDSTRIP2_DATAPIN, NUM_LEDS2,
    static_cast<PicoLed::DataFormat>(FORMAT_LEDS2));

static inline EXPANDER::TCA9534DWR ioExpander(I2C_PORT, TCA9534_ADDR, I2C_SDA_PIN, I2C_SCL_PIN, I2C_BAUDRATE);

void uart_task(void *Pvarg)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    init_cli_interface();
    while (true)
    {
        periodic_cli_handler();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

static void mainTask(void *params)
{
    SERIAL::uart_buffer_t rxBuffer;

    while (1)
    {
        // Check for received UART data without blocking
        if (uartStreamer.uart_check_rx_non_blocking(&rxBuffer, sizeof(rxBuffer), pdMS_TO_TICKS(100)))
        {
            // Process received UART data
            printf("Received UART data: %s\n", rxBuffer.data);
        }
        else
        {
            // Yield to other tasks
            taskYIELD();
        }
    }
}

static void uart_send_task(void *params)
{
    const char *message = "Hello, UART World!\n";
    uartStreamer.uart_send_non_blocking(message);

    while (1)
    {
        uartStreamer.uart_send_non_blocking(message); // Non-blocking send
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

static void ledstrip_task(void *params)
{
    ledStrips.clear();

    while (1)
    {
        /* Automatic fade in and fade out*/
        ledStrips.fadeInOut(PicoLed::RGB(255, 255, 0), ledsOnTime, fadeIntervalTime);
        vTaskDelay(ledsOffTime);

        /* Manually fading in or out */
        ledStrips.fadeIn(PicoLed::RGB(0, 128, 128), fadeIntervalTime);
        vTaskDelay(ledsOnTime);
        ledStrips.fadeOut(PicoLed::RGB(128, 128, 0), fadeIntervalTime);
        vTaskDelay(ledsOffTime);
        

        /* Effect fade wont work, no delay can be seen */
        // ledStrips.fadeValue(PicoLed::RGB(0, 0, 255), 255);
        // vTaskDelay(pdMS_TO_TICKS(3000));
        // ledStrips.effectFade(PicoLed::RGB(255, 0, 0), 128);
        // vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

int main()
{
    stdio_init_all();
    lv_init();
    lv_port_disp_init();

    ledStrips.setBrightness(LED_BRIGHTNESS);
    ledStrips.clear();

    lv_disp_t *disp1 = lv_disp_get_default();
    lv_disp_t *disp2 = lv_disp_get_next(disp1);

    DISPLAY::EyeDisplayDriver driver(disp1, disp2);

    // xTaskCreate(driver.displayHandler, "display_task", 800, &driver, 1, NULL);
    // xTaskCreate(driver.runEyeControlHandle, "uart_task", 1700, &driver, 2, NULL);
    // xTaskCreate(uart_task, "uart_task", 360, nullptr, 1, nullptr);
    xTaskCreate(ledstrip_task, "ledstrip_task", 1700, &ledStrips, 1, NULL);

    vTaskStartScheduler();

    return 0;
}
