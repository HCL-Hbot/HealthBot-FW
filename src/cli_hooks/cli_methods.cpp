#include "cli_methods.hpp"
#include "embedded_cli.h"
#include <stdio.h>
#include <stdlib.h>     /* atoi */

#define ENUM_CLASS_VAL(x) (static_cast<uint8_t>(x))

int curr_motor_pitch_pos = 0;
int curr_motor_yaw_pos = 0;

void display_ena(EmbeddedCli *cli, char *args, void *context) {
        
}

void display_anim(EmbeddedCli *cli, char *args, void *context) {

}

void display_move(EmbeddedCli *cli, char *args, void *context) {
  
}

void motor_cfg(EmbeddedCli *cli, char *args, void *context) {

}

void motor_move(EmbeddedCli *cli, char *args, void *context) {
// uint8_t count = embeddedCliGetTokenCount(args);
// if(count > 1) {
//     const int obj_id  = atoi(embeddedCliGetToken(args, 1));
//     const int yaw = atoi(embeddedCliGetToken(args, 2));
//     const int pitch = atoi(embeddedCliGetToken(args, 3));
//     const int velocity = atoi(embeddedCliGetToken(args, 4));
//     if(obj_id > ENUM_CLASS_VAL(screen_objects::screen_obj_alt_1)) {
//         embeddedCliPrint(cli, "!E object id parameter should be a number between 0 and 9!");
//         return;
//     }
    
//     if(yaw > 360 || yaw < 0) {
//         embeddedCliPrint(cli, "!E yaw should be a number between 0 and 360 degrees!");
//         return;
//     }
    
//     if(pitch > 360 || pitch < 0) {
//         embeddedCliPrint(cli, "!E pitch should be a number between 0 and 360 degrees!");
//         return;
//     }
    
//     if(velocity > 1000 || velocity < 0) {
//         embeddedCliPrint(cli, "!E duration should be a number between 0 and 1000 ms!");
//     }


// } else {
//     embeddedCliPrint(cli, "!E Invalid number of parameters entered! Should be D104 X Y T");
// }
}


void led_ena(EmbeddedCli *cli, char *args, void *context) {

}

void led_color(EmbeddedCli *cli, char *args, void *context) {

}

void led_intensity(EmbeddedCli *cli, char *args, void *context) {

}

void radar_human_present(EmbeddedCli *cli, char *args, void *context) {

}

void radar_stream(EmbeddedCli *cli, char *args, void *context) {

}

void status(EmbeddedCli *cli, char *args, void *context) {

}


