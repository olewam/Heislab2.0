/**
  * @file
  * @brief implementation of safetyfunctions
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "elevator_logic.h"
#include "button_lights_doors.h"

void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

void read_obstruction_signal(){
    if(hardware_read_obstruction_signal()){
        hardware_command_stop_light(1);
        clear_all_order_lights();
        sigint_handler(1);
    }
    else{
        hardware_command_stop_light(0);
    }
}

void stop_button_pushed(HardwareMovement *current_movement, int current_floor, int UP_list[], int DOWN_list[], _Bool * wrong_dir_flag){
    while(hardware_read_stop_signal()){
        hardware_command_stop_light(1);
        clear_all_orders(UP_list, DOWN_list);
        *current_movement = HARDWARE_MOVEMENT_STOP;
        hardware_command_movement(*current_movement);
        clear_all_order_lights();
    }
    hardware_command_stop_light(0);

  }
