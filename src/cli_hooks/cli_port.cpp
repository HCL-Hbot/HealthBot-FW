#include <cli_port.hpp>
#define EMBEDDED_CLI_IMPL
#include <cli_methods.hpp>

#include "embedded_cli.h"

#include "pico/stdlib.h"
#include <hardware/uart.h>
#include <hardware/gpio.h>

/****************************************************************/
/*                      PORTING functions                       */
/****************************************************************/
EmbeddedCli *cli;

void writeChar(EmbeddedCli *embeddedCli, char c) {
    /* Use the commented putchar for usage with USB Serial! */
    // putchar(c);
    /* Otherwise use this uart_putc function with the related UART interface! */
    uart_putc(uart0, c);
}

void periodic_cli_handler(void) {
    /* Use the commented getchar for usage with USB Serial! */
    // int c = getchar();
    /* Otherwise use this uart_getc function with the related UART interface! */
    int c = uart_getc(uart0);
    embeddedCliReceiveChar(cli, c);
    embeddedCliProcess(cli);
}

void init_cli_interface() {
    
    gpio_set_function(17, GPIO_FUNC_UART);
    gpio_set_function(16, GPIO_FUNC_UART);
    uart_init(uart0, 115200);
    
    // motor_pitch.initPulseGenerator(1, 30, 250);

    EmbeddedCliConfig *config = embeddedCliDefaultConfig();
    config->maxBindingCount = 16;
    cli = embeddedCliNew(config);   
    cli->writeChar = writeChar;

    embeddedCliAddBinding(cli, {
        "D104",          // command name (spaces are not allowed)
        "Moves object on display with id, x,y coordinates; \n D104 2 100 100 moves object 2 to x100, y100",   // Optional help for a command (NULL for no help)
        true,              // flag whether to tokenize arguments (see below)
        nullptr,            // optional pointer to any application context
        display_move               // binding function 
    });

    embeddedCliAddBinding(cli, {
        "D103",          // command name (spaces are not allowed)
        "Turns off/on visibility for object on display",   // Optional help for a command (NULL for no help)
        true,              // flag whether to tokenize arguments (see below)
        nullptr,            // optional pointer to any application context
        display_move               // binding function 
    });

    embeddedCliAddBinding(cli, {
        "M101",          // command name (spaces are not allowed)
        "Moves motor with Pitch, Yaw-angles and speed \n M101 Y10 P20 S100 moves motor to yaw angle 10 degrees, pitch angles 20 degrees with 100mm/sec",   // Optional help for a command (NULL for no help)
        true,              // flag whether to tokenize arguments (see below)
        nullptr,            // optional pointer to any application context
        motor_move               // binding function 
    });

}