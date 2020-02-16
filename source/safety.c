/**
  * @file
  * @brief implementation of safetyfunctions
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"

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

void stop_button_pushed(HardwareMovement current_movement){
  while(hardware_read_stop_signal()){
      hardware_command_stop_light(1);
      hardware_command_movement(HARDWARE_MOVEMENT_STOP);
  }
  hardware_command_movement(current_movement);  //fjern senere, når bestillinger er lagt inn
  hardware_command_stop_light(0);
}

void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

/*
//Pass by reference slik at movement blir redigert i main
void elevator_limits(HardwareMovement &movement){
    if(hardware_read_floor_sensor(0)){
        movement = HARDWARE_MOVEMENT_UP;
        hardware_command_movement(movement);
    }
    if(hardware_read_floor_sensor(HARDWARE_NUMBER_OF_FLOORS - 1)){
       movement = HARDWARE_MOVEMENT_DOWN;
       hardware_command_movement(movement);
    }
}
*/