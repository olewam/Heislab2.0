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

void terminate_elevator(){
    if(hardware_read_obstruction_signal() && (hardware_read_stop_signal())){
        hardware_command_stop_light(1);
        clear_all_order_lights();
        sigint_handler(1);
    }
    else{
        hardware_command_stop_light(0);
    }
}

void stop_button_pushed(HardwareMovement *current_movement, int current_floor, int UP_list[], int DOWN_list[], _Bool * wrong_dir_flag){
    _Bool stop_signal_flag = 0;
    while(hardware_read_stop_signal()){
      hardware_command_stop_light(1);
      hardware_command_stop_light(1);
      clear_all_orders(UP_list, DOWN_list);
      *current_movement = HARDWARE_MOVEMENT_STOP;
      hardware_command_movement(*current_movement);
      clear_all_order_lights();
        if(hardware_read_floor_sensor(current_floor)){
          hardware_command_door_open(1);
          stop_signal_flag = 1;
        }

    }
    if(hardware_read_floor_sensor(current_floor) && (stop_signal_flag ==1)){
      wait_3_seconds(UP_list, DOWN_list, current_floor, current_movement);
    }
    hardware_command_stop_light(0);

  }
