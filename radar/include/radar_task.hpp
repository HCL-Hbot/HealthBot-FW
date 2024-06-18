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
#ifndef RADAR_TASK_HPP
#define RADAR_TASK_HPP

#include "FreeRTOS.h"
#include "queue.h"
#include "command_defines.hpp"
#include "brainboard_driver.hpp"

namespace COM {
class BrainBoardDriver; // Forward declaration of BrainBoardDriver
}

namespace RADAR {

extern QueueHandle_t radarCommandQueue;

void startRadarTasks(COM::BrainBoardDriver* driver);

} // namespace RADAR
#endif // RADAR_TASK_HPP