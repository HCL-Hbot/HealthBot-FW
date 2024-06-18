/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>
#include <hardware/spi.h>
#include <hardware/gpio.h>
#include <board_defines.h>
#include "display/GC9A01.h"

/*********************
 *      DEFINES
 *********************/
#define MY_DISP_HOR_RES 240
#define MY_DISP_VER_RES 240
#define DISPLAY1_ID 0
#define DISPLAY2_ID 1

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    disp_init();

    /* Initialize display buffers */
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 10]; // A buffer for 10 rows
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);

    static lv_disp_draw_buf_t draw_buf_dsc_2;
    static lv_color_t buf_2[MY_DISP_HOR_RES * 10]; // A buffer for 10 rows for the second display
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2, NULL, MY_DISP_HOR_RES * 10);

    /* Initialize the first display driver */
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.user_data = (void*)0; // ID for the first display
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;
    disp_drv.flush_cb = GC9A01_flush_displays;
    disp_drv.draw_buf = &draw_buf_dsc_1;
    disp_ref_1 = lv_disp_drv_register(&disp_drv);

    /* Initialize the second display driver */
    static lv_disp_drv_t disp_drv_2;
    lv_disp_drv_init(&disp_drv_2);
    disp_drv_2.user_data = (void*)1; // ID for the second display
    disp_drv_2.hor_res = MY_DISP_HOR_RES;
    disp_drv_2.ver_res = MY_DISP_VER_RES;
    disp_drv_2.flush_cb = GC9A01_flush_displays;
    disp_drv_2.draw_buf = &draw_buf_dsc_2;
    disp_ref_2 = lv_disp_drv_register(&disp_drv_2);

    lv_disp_set_default(disp_ref_2);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/

static void disp_init(void)
{
    // Common SPI functions
    gpio_set_function(DISPLAY_MISO, GPIO_FUNC_SPI);
    gpio_set_function(DISPLAY_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(DISPLAY_SCK, GPIO_FUNC_SPI);

    // Initialize the first display
    gpio_set_function(DISPLAY_CS1, GPIO_FUNC_SIO);
    gpio_set_dir(DISPLAY_CS1, GPIO_OUT);
    gpio_put(DISPLAY_CS1, 1); // Deselect the first display

    gpio_set_function(DISPLAY_CMD, GPIO_FUNC_SIO);
    gpio_set_dir(DISPLAY_CMD, GPIO_OUT);

    gpio_set_function(DISPLAY_RST1, GPIO_FUNC_SIO);
    gpio_set_dir(DISPLAY_RST1, GPIO_OUT);

    // Initialize the SPI for the first display
    spi_init(SPI_INST, GC9A01_SPI_BAUD);
    GC9A01_init(DISPLAY1_ID); // You might want to pass an identifier if both displays use different initial settings

    // Initialize the second display
    // Assume DISPLAY_CS2 is the GPIO pin number for the second display's CS
    gpio_set_function(DISPLAY_CS2, GPIO_FUNC_SIO);
    gpio_set_dir(DISPLAY_CS2, GPIO_OUT);
    gpio_put(DISPLAY_CS2, 1); // Deselect the second display

    GC9A01_init(DISPLAY2_ID); // Again, customize if needed
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
// static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
// {
//     if(disp_flush_enabled) {
//         /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

//         int32_t x;
//         int32_t y;
//         for(y = area->y1; y <= area->y2; y++) {
//             for(x = area->x1; x <= area->x2; x++) {
//                 /*Put a pixel to the display. For example:*/
//                 /*put_px(x, y, *color_p)*/
//                 color_p++;
//             }
//         }
//     }

//     /*IMPORTANT!!!
//      *Inform the graphics library that you are ready with the flushing*/
//     lv_disp_flush_ready(disp_drv);
// }

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
