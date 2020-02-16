/**
  * @file 
  * @brief implementation of order functions
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"

void set_UP_list(int UP_list[]){
  for(int f = 0; f < (HARDWARE_NUMBER_OF_FLOORS - 1); f++){
    if(hardware_read_order(f, HARDWARE_ORDER_UP)){
      hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
      UP_list[f] = 1;
    }
  }
}

void set_DOWN_list(int DOWN_list[]){
  for(int f = 1; f < (HARDWARE_NUMBER_OF_FLOORS); f++){
    if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
      hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
      DOWN_list[f-1] = 1;
    }
  }
}

/*
//dette er feil
void handle_inside_order(int current_floor, int UP_list[], int DOWN_list[]){
  for(int f = 0; f < (HARDWARE_NUMBER_OF_FLOORS); f++){
    if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
      if(current_floor < f){
        UP_list[f] = 1;
      }
      else if(current_floor > f){
        DOWN_list[f - 1] = 1;
      }
      else{
          continue;
      }
    }
  }
}
*/
