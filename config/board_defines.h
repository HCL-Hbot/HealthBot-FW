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

#ifndef BOARD_DEFINES_H
#define BOARD_DEFINES_H

#define DISPLAY1_ID             0 
#define DISPLAY2_ID             1

// Select the board
// #define SELECTED_BOARD                              
#define UNIVERSAL_CONNECTOR_BOARD_REV1  1

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

/* MMWAVE_RADAR */
#define SPI_INST_RADAR      spi0
#define SPI_SCK_RADAR       6  // SPI Clock
#define SPI_MOSI_RADAR      3  // Master Out Slave In (MOSI)
#define SPI_MISO_RADAR      4  // Master In Slave Out (MISO)
#define SPI_CS_RADAR        2  // Chip Select (CS)
#define SPI_RS_RADAR        0  // Reset Line

#elif UNIVERSAL_CONNECTOR_BOARD_REV1

#define SPI_INST                spi1
#define GC9A01_SPI_BAUD         20000000
#define SPI_BAUD_RATE_RADAR     20000000
#define UART_BAUD_RATE          115200

#define UART_INST               uart0

/* SERIAL UART PINS */
#define UART_TX_PICO            0
#define UART_RX_PICO            1

/* UART MUX REFS: */
// #define UART_TX_PICO        4
// #define UART_RX_PICO        5

/* RADAR PINS */
#define SPI_INST_RADAR      spi1
#define RADAR_INT           6
#define SPI_SCK_RADAR       DISPLAY_SCK
#define SPI_MISO_RADAR      12
#define SPI_MOSI_RADAR      DISPLAY_MOSI
#define SPI_RS_RADAR        18
#define SPI_CS_RADAR        17

/* DISPLAYS */
#define DISPLAY_RST2        7
#define DISPLAY_RST1        MUX_INH // Originallay: 7, after patch: 16, check PATH note.
#define DISPLAY_CMD         8
#define DISPLAY_CS1         9
#define DISPLAY_CS2         13
#define DISPLAY_SCK         10
#define DISPLAY_MOSI        11
#define DISPLAY_MISO        SPI_MISO_RADAR // Only initialized, not used.

/* LEDSTRIP */
#define LEDSTRIP1_DATAPIN   14
#define LEDSTRIP2_DATAPIN   15

/* LED CHEECKS */
#define PEXP5_LED_CHEECKS   PEXP5
#define PEXP4_LED_MOUTH     PEXP4       

/* MOTOR PINS */
#define MOTOR2_DIR          22
#define MOTOR2_STEP         21
#define MOTOR1_DIR          20
#define MOTOR1_STEP         19

#define PEXP0_MOTOR1_ENABLE PEXP0    
#define PEXP1_MOTOR2_ENABLE PEXP1

#define PEXP2_MOTOR1_SW     PEXP2
#define PEXP3_MOTOR2_SW     PEXP3

/* IO EXPANDER */
#define IO_EXPANDER_INT     28
#define IO_EXPANDER_SCL     27
#define IO_EXPANDER_SDA     26

#define PEXP0               0
#define PEXP1               1
#define PEXP2               2
#define PEXP3               3
#define PEXP4               4
#define PEXP5               5
#define PEXP6               6
#define PEXP7               7

/* UART MUX */
#define MUX_INH             16
#define PEXP7_MUX_A         PEXP7     
#define PEXP6_MUX_B         PEXP6 

#else 
#error "Please select a board, No pin defines found."

#endif // SELECTED_BOARD
#endif // BOARD_DEFINES_H