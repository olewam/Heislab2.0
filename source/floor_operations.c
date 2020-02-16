/**
  * @file
  * @brief Implementation of floor operation functions
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"

int current_floor(int temp_floor)
{
  if(hardware_read_floor_sensor(1)){ temp_floor = 1; }
  if(hardware_read_floor_sensor(2)){ temp_floor = 2; }
  if(hardware_read_floor_sensor(3)){ temp_floor = 3; }
  if(hardware_read_floor_sensor(0)){ temp_floor = 0; }
  return temp_floor;
}


void set_current_floor_light(int temp_floor)
{
  hardware_command_floor_indicator_on(temp_floor);
}

void set_and_clear_order_lights(){
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
       // Internal orders
       if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
           hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
       } else {
           hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 0);
       }

       // Orders going up
       if(hardware_read_order(f, HARDWARE_ORDER_UP)){
           hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
       } else {
           hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 0);
       }

       // Orders going down
       if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
           hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
       } else {
           hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 0);
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

HardwareMovement choose_init_direction(int UP_list[], int DOWN_list[]){
    //Need to check if requested floor is in the opposite direction of the requested direction
    for(int i = 0; i < 3; i++){
      if(UP_list[i] == 1){
        return HARDWARE_MOVEMENT_UP;
      }
      else if(DOWN_list[i] == 1){ //Blir indekseringen for UP og DOWN list feil???
        return HARDWARE_MOVEMENT_DOWN;
      }
    }
    return HARDWARE_MOVEMENT_STOP;
}

/*
New suggestion for the function:

HardwareMovement choose_init_direction(int UP_list[], int DOWN_list[], int current_floor){
    for(int i = 0; i < 3; i++){
      if(UP_list[i] == 1){
        if(i < current_floor){
          return HARDWARE_MOVEMENT_DOWN;
        } else if {
          return HARDWARE_MOVEMENT_UP;
      }
    }

    if(DOWN_list[i] == 1){
        if(i > current_floor){
            return HARDWARE_MOVEMENT_UP;
        } else if {
            return HARDWARE_MOVEMENT_DOWN;
        }
    }
  }
  return HARDWARE_MOVEMENT_STOP;
}
*/
