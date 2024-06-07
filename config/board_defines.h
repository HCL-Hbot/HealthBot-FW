#ifndef BOARD_DEFINES_H
#define BOARD_DEFINES_H

#define SPI_INST            spi0
#define GC9A01_SPI_BAUD     20000000

#define DISPLAY1_ID 0 
#define DISPLAY2_ID 1

// Select the board
#define SELECTED_BOARD                  1
#define UNIVERSAL_CONNECTOR_BOARD_REV1  0

#ifdef SELECTED_BOARD

/* DISPLAY HW */
#define DISPLAY_MISO        4
#define DISPLAY_SCK         2
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

/* MOTOR PINS */
#define MOTOR1_ENABLE       18
#define MOTOR1_STEP         19
#define MOTOR1_DIR          20

#define MOTOR2_ENABLE       6
#define MOTOR2_STEP         21
#define MOTOR2_DIR          22

/* LEDSTRIP PINS*/
#define LEDSTRIP1_DATAPIN    13
#define LEDSTRIP2_DATAPIN    15

#elif UNIVERSAL_CONNECTOR_BOARD_REV1 

#else 
#error "Please select a board, No pin defines found."

#endif // SELECTED_BOARD
#endif // BOARD_DEFINES_H