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