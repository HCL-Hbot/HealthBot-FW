#ifndef BOARD_DEFINES_H
#define BOARD_DEFINES_H

#define SPI_INST            spi0
#define GC9A01_SPI_BAUD     20000000

#define DISPLAY1_ID 0 
#define DISPLAY2_ID 1

/* DISPLAY HW */
#define DISPLAY_MISO        4
#define DISPLAY_SCK         6
#define DISPLAY_MOSI        3
#define DISPLAY_CMD         5
// Display Left: 
#define DISPLAY_CS1         0
#define DISPLAY_RST1        1
// Display Right: 
#define DISPLAY_CS2         7
#define DISPLAY_RST2        8

/* SERIAL UART PINS */
#define UART_TX_PIN         16
#define UART_RX_PIN         17
#define UART_BAUD_RATE      115200

#endif