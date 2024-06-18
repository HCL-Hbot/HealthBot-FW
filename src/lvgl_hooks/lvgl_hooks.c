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

#include "lvgl_hooks.h"
#include <hardware/gpio.h>
#include <hardware/spi.h>

void display_cmd_data(const uint8_t val) {
    gpio_put(DISPLAY_CMD, val);
}

void display_spi_cs(uint8_t display_id, const uint8_t val) {
    if (display_id == DISPLAY1_ID) {
        gpio_put(DISPLAY_CS1, val);
    } else if (display_id == DISPLAY2_ID) {
        gpio_put(DISPLAY_CS2, val);
    }
}

void display_spi_rst(uint8_t display_id, const uint8_t val) {
    if (display_id == DISPLAY1_ID) {
        gpio_put(DISPLAY_RST1, val);
    } else if (display_id == DISPLAY2_ID) {
        gpio_put(DISPLAY_RST2, val);
    }
}

void display_spi_wr_byte(uint8_t display_id, const uint8_t data) {
    display_spi_cs(display_id, 0);  // Select the display
    spi_write_blocking(SPI_INST, &data, 1);
    display_spi_cs(display_id, 1);  // Deselect the display
}

void display_spi_wr_array(uint8_t display_id, const char *src, size_t n) {
    display_spi_cs(display_id, 0);  // Select the display
    spi_write_blocking(SPI_INST, (uint8_t *)src, n);
    display_spi_cs(display_id, 1);  // Deselect the display
}