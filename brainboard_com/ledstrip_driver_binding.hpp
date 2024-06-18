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

#ifndef LEDSTRIP_DRIVER_BINDING_HPP
#define LEDSTRIP_DRIVER_BINDING_HPP

#include "embedded_cli.h"
#include "command_defines.hpp"

#include "FreeRTOS.h"
#include "queue.h"

namespace COM {

extern QueueHandle_t ledStripCommandQueue;

/* 1 - COMMAND DEFINES */
enum class LedstripCommandType {
    SET_RGB             = 0,
    ANIM_BLINK          = 1,
    ANIM_FADE_OUT       = 2,
    ANIM_FADE_IN        = 3,
};

struct LedstripCommand {
    LedstripCommandType type;
    int ledstripId;

    union {
        struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        } rgb;
        struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t duration;
        } anim_blink;
        struct {
            uint8_t duration;
        } anim_fade_out;
        struct {
            uint8_t duration;
        } anim_fade_in;
    } data;
};

/* 2 - HANDLERS */
static void handleSetRgb(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 4) {
        const char* ledstripIdStr = embeddedCliGetToken(args, 1);
        const char* rStr = embeddedCliGetToken(args, 2);
        const char* gStr = embeddedCliGetToken(args, 3);
        const char* bStr = embeddedCliGetToken(args, 4);

        const int ledstripId = atoi(ledstripIdStr);
        const int r = atoi(rStr);
        const int g = atoi(gStr);
        const int b = atoi(bStr);

        if (ledstripId != -1 && r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255) {
            LedstripCommand command;
            command.type = LedstripCommandType::SET_RGB;
            command.ledstripId = ledstripId;
            command.data.rgb = { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b) };
            xQueueSend(ledStripCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for LEDSTRIP SET_RGB command\n");
        }
    } else {
        printf("Invalid argument count for LEDSTRIP SET_RGB command\n");
    }
}

static void handleAnimBlink(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 5) {
        const char* ledstripIdStr = embeddedCliGetToken(args, 1);
        const char* rStr = embeddedCliGetToken(args, 2);
        const char* gStr = embeddedCliGetToken(args, 3);
        const char* bStr = embeddedCliGetToken(args, 4);
        const char* durationStr = embeddedCliGetToken(args, 5);

        const int ledstripId = atoi(ledstripIdStr);
        const int r = atoi(rStr);
        const int g = atoi(gStr);
        const int b = atoi(bStr);
        const int duration = atoi(durationStr);

        if (ledstripId != -1 && r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255 && duration > 0) {
            LedstripCommand command;
            command.type = LedstripCommandType::ANIM_BLINK;
            command.ledstripId = ledstripId;
            command.data.anim_blink = { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), static_cast<uint8_t>(duration) };
            xQueueSend(ledStripCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for LEDSTRIP ANIM_BLINK command\n");
        }
    } else {
        printf("Invalid argument count for LEDSTRIP ANIM_BLINK command\n");
    }
}

static void handleAnimFadeOut(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 2) {
        const char* ledstripIdStr = embeddedCliGetToken(args, 1);
        const char* durationStr = embeddedCliGetToken(args, 2);

        const int ledstripId = atoi(ledstripIdStr);
        const int duration = atoi(durationStr);

        if (ledstripId != -1 && duration > 0) {
            LedstripCommand command;
            command.type = LedstripCommandType::ANIM_FADE_OUT;
            command.ledstripId = ledstripId;
            command.data.anim_fade_out.duration = static_cast<uint8_t>(duration);
            xQueueSend(ledStripCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for LEDSTRIP ANIM_FADE_OUT command\n");
        }
    } else {
        printf("Invalid argument count for LEDSTRIP ANIM_FADE_OUT command\n");
    }
}

static void handleAnimFadeIn(EmbeddedCli* cli, char* args, void* context) {
    const uint16_t argCount = embeddedCliGetTokenCount(args);
    if (argCount == 2) {
        const char* ledstripIdStr = embeddedCliGetToken(args, 1);
        const char* durationStr = embeddedCliGetToken(args, 2);

        const int ledstripId = atoi(ledstripIdStr);
        const int duration = atoi(durationStr);

        if (ledstripId != -1 && duration > 0) {
            LedstripCommand command;
            command.type = LedstripCommandType::ANIM_FADE_IN;
            command.ledstripId = ledstripId;
            command.data.anim_fade_in.duration = static_cast<uint8_t>(duration);
            xQueueSend(ledStripCommandQueue, &command, portMAX_DELAY);
        } else {
            printf("Invalid arguments for LEDSTRIP ANIM_FADE_IN command\n");
        }
    } else {
        printf("Invalid argument count for LEDSTRIP ANIM_FADE_IN command\n");
    }
}

/* 3 - CLI BINDINGS */
static void addLedstripCliBindings(EmbeddedCli* cli) {
    addCliCommandBinding(cli, "LS101",
        "Set RGB: LEDSTRIP SET_RGB <LedstripID> <R> <G> <B>", 
        true, nullptr, handleSetRgb);

    addCliCommandBinding(cli, "LS102", 
        "Blink animation: LEDSTRIP ANIM_BLINK <LedstripID> <R> <G> <B> <Duration>", 
        true, nullptr, handleAnimBlink);

    addCliCommandBinding(cli, "LS103", 
        "Fade out animation: LEDSTRIP ANIM_FADE_OUT <LedstripID> <Duration>", 
        true, nullptr, handleAnimFadeOut);

    addCliCommandBinding(cli, "LS104", 
        "Fade in animation: LEDSTRIP ANIM_FADE_IN <LedstripID> <Duration>", 
        true, nullptr, handleAnimFadeIn);
}

} // namespace COM
#endif // LEDSTRIP_DRIVER_BINDING_HPP