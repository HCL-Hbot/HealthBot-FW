#include "radar_task.hpp"
#include "bgt60ltr11XXX_driver.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

namespace RADAR {

QueueHandle_t radarDataQueue;

typedef struct {
    uint16_t valueI;
    uint16_t valueQ;
} RadarData;

static TaskHandle_t radarTaskHandle = NULL;

void radarReadingTask(void* pvParameters) {
    BGT60::BGT60_DRIVER radarDriver{};
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    radarDriver.initCwMode();

    uint16_t valueQ = 0;
    uint16_t valueI = 0;

    while (true) {
        valueQ = radarDriver.read_AdChannel(BGT60::ADC_REG_CHANNELS::IFQ);
        valueI = radarDriver.read_AdChannel(BGT60::ADC_REG_CHANNELS::IFI);
        
        if (valueQ != 0 && valueI != 0) {
            RadarData data = {valueI, valueQ};
            xQueueSend(radarDataQueue, &data, portMAX_DELAY);
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void handleRadarCommand(const COM::RadarCommand& command, COM::BrainBoardDriver* driver) {
    RadarData data;

    switch (command.type) {
        case COM::RadarCommandType::INIT:
            break;
        case COM::RadarCommandType::START:
            if (radarTaskHandle == NULL) {
                xTaskCreate(radarReadingTask, "RadarReadingTask", 1024, NULL, tskIDLE_PRIORITY + 1, &radarTaskHandle);
            }
            break;
        case COM::RadarCommandType::STOP:
            if (radarTaskHandle != NULL) {
                vTaskDelete(radarTaskHandle);
                radarTaskHandle = NULL;
            }
            break;
        case COM::RadarCommandType::SEND_DATA:
            while (xQueueReceive(radarDataQueue, &data, 0)) {
                driver->sendRadarData(data.valueI, data.valueQ);
            }
            break;
        default:
            break;
    }
}

void radarTask(void* pvParameters) {
    COM::BrainBoardDriver* driver = static_cast<COM::BrainBoardDriver*>(pvParameters);
    COM::RadarCommand command;

    while (true) {
        if (xQueueReceive(COM::radarCommandQueue, &command, portMAX_DELAY)) {
            handleRadarCommand(command, driver);
        }
    }
}

void startRadarTasks(COM::BrainBoardDriver* driver) {
    radarDataQueue = xQueueCreate(10, sizeof(RadarData));
    xTaskCreate(radarTask, "RadarTask", 1024, driver, tskIDLE_PRIORITY + 1, NULL);
}

} // namespace COM
