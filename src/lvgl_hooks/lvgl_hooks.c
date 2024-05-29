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