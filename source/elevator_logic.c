/**
  * @file
  * @brief implementation of functions to create the elevator logic
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"

int find_default_floor(){
  while(1){
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
      if(hardware_read_floor_sensor(f)){
          hardware_command_movement(HARDWARE_MOVEMENT_STOP);
          return f;
      }
      hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
  }
  return 0; //Never happens
}

int current_floor(int temp_floor)
{
  if(hardware_read_floor_sensor(1)){ temp_floor = 1; }
  if(hardware_read_floor_sensor(2)){ temp_floor = 2; }
  if(hardware_read_floor_sensor(3)){ temp_floor = 3; }
  if(hardware_read_floor_sensor(0)){ temp_floor = 0; }
  return temp_floor;
}

HardwareMovement choose_init_direction(int UP_list[], int DOWN_list[], int current_floor){
    for(int i = 0; i < sizeof(HardwareOrder); i++){
      if(UP_list[i] == 1){
        if(i < current_floor){
          return HARDWARE_MOVEMENT_DOWN;
        } else {
          return HARDWARE_MOVEMENT_UP;
      }
    }

    if(DOWN_list[i] == 1){
        if((i + 1) > current_floor){
            return HARDWARE_MOVEMENT_UP;
        } else {
            return HARDWARE_MOVEMENT_DOWN;
        }
    }
  }
  return HARDWARE_MOVEMENT_STOP;
}

HardwareMovement stop_UP_list_elevator(int UP_list[], int current_floor, HardwareMovement current_movement){
  for(int i = 0; i < sizeof(HardwareOrder); i++){
    if((UP_list[i] == 1) && (i == current_floor)){
      UP_list[i] = 0;
      hardware_command_order_light(i, HARDWARE_MOVEMENT_UP, 0);
      return HARDWARE_MOVEMENT_STOP;
    }
  }
  return current_movement;
}

HardwareMovement stop_DOWN_list_elevator(int DOWN_list[], int current_floor, HardwareMovement current_movement){
  for(int i = 0; i < sizeof(HardwareOrder); i++){
    if((DOWN_list[i] == 1) && ((i + 1) == current_floor)){
      DOWN_list[i] = 0;
      hardware_command_order_light((i + 1), HARDWARE_MOVEMENT_DOWN, 0);
      return HARDWARE_MOVEMENT_STOP;
    }
  }
  return current_movement;
}

void clear_all_orders(int UP_list[], int DOWN_list[]){
  for(int i = 0; i < sizeof(HardwareOrder); i++){
    UP_list[i] = 0;
    DOWN_list[i] = 0;
  }
}
