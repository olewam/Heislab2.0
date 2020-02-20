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

void stop_button_pushed(HardwareMovement current_movement, int current_floor, int UP_list[], int DOWN_list[], _Bool * wrong_dir_flag){
  _Bool stop_button_flag = 0;
  while(hardware_read_stop_signal()){
      hardware_command_stop_light(1);
      clear_all_orders(UP_list, DOWN_list);
      current_movement = HARDWARE_MOVEMENT_STOP;
      hardware_command_movement(current_movement);
      clear_all_order_lights();
      stop_button_flag = 1;
  }
  hardware_command_stop_light(0);
  if(stop_button_flag == 1){
    _Bool loop_flag = 0;
    while(!loop_flag){
      set_UP_list(UP_list);
      set_DOWN_list(DOWN_list);
      handle_inside_order(UP_list, DOWN_list);
      choose_init_direction(UP_list, DOWN_list, current_floor, wrong_dir_flag);
      for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
        if(UP_list[i] == 1 || DOWN_list[i] == 1){
          loop_flag = 1;
          break;
        }
      }
    }
  }
}
