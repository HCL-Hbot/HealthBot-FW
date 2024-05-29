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