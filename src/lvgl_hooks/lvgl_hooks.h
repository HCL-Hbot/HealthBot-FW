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

#ifndef LVGL_HOOKS_H
#define LVGL_HOOKS_H

#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include "board_defines.h"

void display_cmd_data(const uint8_t val);

void display_spi_rst(uint8_t display_id, const uint8_t val);

void display_spi_cs(uint8_t display_id, const uint8_t val);

void display_spi_wr_byte(uint8_t display_id, const uint8_t data);

void display_spi_wr_array(uint8_t display_id, const char *src, size_t n);

void display_spi_mode(const uint8_t bits, const uint8_t mode);

#endif