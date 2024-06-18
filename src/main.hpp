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

#ifndef MAIN_HPP
#define MAIN_HPP

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

#include <ledstrip_driver.hpp>
#include <Effects/Fade.hpp>
#include "PicoLedEffect.hpp"

#include <brainboard_driver.hpp>
#include <step_motor_manager.hpp>
#include <ledstrip_manager.hpp>

#include <bgt60ltr11XXX_driver.hpp>
#include <expander_driver.hpp>
#include <radar_task.hpp>
#include <led_driver.hpp>

#define CLI_LEDSTRIP_ONE_REF    1
#define CLI_LEDSTRIP_TWO_REF    2

#define CLI_MOTOR_ONE_REF       1
#define CLI_MOTOR_TWO_REF       2

#endif // MAIN_HPP