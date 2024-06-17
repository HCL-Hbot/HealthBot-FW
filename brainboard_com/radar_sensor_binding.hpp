#ifndef RADAR_DRIVER_BINDING_HPP
#define RADAR_DRIVER_BINDING_HPP

#include "embedded_cli.h"
#include "command_defines.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "bgt60ltr11XXX_driver.hpp"

namespace COM {

extern QueueHandle_t radarCommandQueue;

/* 1 - COMMAND DEFINES */
enum class RadarCommandType {
    INIT                = 0,
    START               = 1,
    STOP                = 2,
    SEND_DATA           = 3,
};

struct RadarCommand {
    RadarCommandType type;
};

/* 2 - HANDLERS */
static void handleRadarInit(EmbeddedCli* cli, char* args, void* context) {
    RadarCommand command;
    command.type = RadarCommandType::INIT;
    xQueueSend(radarCommandQueue, &command, portMAX_DELAY);
}

static void handleRadarStart(EmbeddedCli* cli, char* args, void* context) {
    RadarCommand command;
    command.type = RadarCommandType::START;
    xQueueSend(radarCommandQueue, &command, portMAX_DELAY);
}

static void handleRadarStop(EmbeddedCli* cli, char* args, void* context) {
    RadarCommand command;
    command.type = RadarCommandType::STOP;
    xQueueSend(radarCommandQueue, &command, portMAX_DELAY);
}

static void handleRadarSendData(EmbeddedCli* cli, char* args, void* context) {
    RadarCommand command;
    command.type = RadarCommandType::SEND_DATA;
    xQueueSend(radarCommandQueue, &command, portMAX_DELAY);
}

/* 3 - CLI BINDINGS */
static void addRadarCliBindings(EmbeddedCli* cli) {
    addCliCommandBinding(cli, "R101", "Initialize radar", true, nullptr, handleRadarInit);
    addCliCommandBinding(cli, "R102", "Start radar", true, nullptr, handleRadarStart);
    addCliCommandBinding(cli, "R103", "Stop radar", true, nullptr, handleRadarStop);
    addCliCommandBinding(cli, "R104", "Send radar data", true, nullptr, handleRadarSendData);
}

} // namespace COM
#endif // RADAR_DRIVER_BINDING_HPP