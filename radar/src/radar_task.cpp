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
