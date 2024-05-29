#ifndef CLI_METHODS_HPP
#define CLI_METHODS_HPP
#include <embedded_cli.h>

void display_ena(EmbeddedCli *cli, char *args, void *context);

void display_move(EmbeddedCli *cli, char *args, void *context);

void display_anim(EmbeddedCli *cli, char *args, void *context);

void motor_cfg(EmbeddedCli *cli, char *args, void *context);

void motor_move(EmbeddedCli *cli, char *args, void *context);

void led_ena(EmbeddedCli *cli, char *args, void *context);

void led_color(EmbeddedCli *cli, char *args, void *context);

void led_intensity(EmbeddedCli *cli, char *args, void *context);

void radar_human_present(EmbeddedCli *cli, char *args, void *context);

void radar_stream(EmbeddedCli *cli, char *args, void *context);

void status(EmbeddedCli *cli, char *args, void *context);
#endif  /* CLI_METHODS_HPP */