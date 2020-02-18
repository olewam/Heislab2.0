/**
  * @file
  * @brief implementation of order functions
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"

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



//dette er feil
void handle_inside_order(int UP_list[], int DOWN_list[]){
  for(int f = 0; f < (HARDWARE_NUMBER_OF_FLOORS); f++){
    if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
      /*if(current_floor >= f){
        DOWN_list[f] = 1;
      }
      if(current_floor <= f){
        UP_list[f] = 1;
      }*/
      UP_list[f] = 1;
      DOWN_list[f] = 1;
    }
  }
}
