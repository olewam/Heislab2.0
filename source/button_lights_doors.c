/**
  * @file
  * @brief implementation of functions for buttons, lights and doors
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include <time.h>

void set_current_floor_light(int current_floor)
{
  hardware_command_floor_indicator_on(current_floor);
}

void set_order_lights(){
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
       if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
           hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
       }
       if(hardware_read_order(f, HARDWARE_ORDER_UP)){
           hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
       }
       if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
           hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
       }
   }
}

void set_UP_list(int UP_list[]){
  for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    if(hardware_read_order(f, HARDWARE_ORDER_UP)){
      UP_list[f] = 1;
    }
  }
}

void set_DOWN_list(int DOWN_list[]){
  for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
    if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
      DOWN_list[f] = 1;
    }
  }
}

void set_INSIDE_order(int UP_list[], int DOWN_list[]){
  for(int f = 0; f < (HARDWARE_NUMBER_OF_FLOORS); f++){
    if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){

      UP_list[f] = 1;
      DOWN_list[f] = 1;
    }
  }
}

void wait_3_seconds(int UP_list[], int DOWN_list[], int current_floor, HardwareMovement *current_movement){
  double initTime = clock();
  _Bool obst_flag = 0;
  set_current_floor_light(current_floor);
  hardware_command_door_open(1);
  while(1){
    *current_movement = HARDWARE_MOVEMENT_STOP;
    hardware_command_movement(*current_movement);
    set_order_lights();
    set_UP_list(UP_list);
    set_DOWN_list(DOWN_list);
    set_INSIDE_order(UP_list, DOWN_list);
    while(hardware_read_obstruction_signal()){
      obst_flag = 1;
      continue;
    }
    if(obst_flag == 1){
      wait_3_seconds(UP_list, DOWN_list, current_floor, current_movement);
    }

    if((clock() - initTime)/CLOCKS_PER_SEC >= 3){
      hardware_command_door_open(0);
      break;
    }
  }
}

void clear_all_order_lights(){
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}
