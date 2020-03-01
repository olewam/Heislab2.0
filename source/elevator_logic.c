/**
  * @file
  * @brief implementation of functions to create the elevator logic
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "buttons_lights_doors.h"
#include <time.h>
#include <stdbool.h>

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
  return 0;
}

void above_or_below(_Bool *above_flag, HardwareMovement current_movement){
  if(current_movement == HARDWARE_MOVEMENT_UP){
    *above_flag = 1;
  } else if (current_movement == HARDWARE_MOVEMENT_DOWN){
    *above_flag = 0;
  }
}

int read_current_floor(int current_floor, _Bool *above_flag, HardwareMovement current_movement)
{
  if(hardware_read_floor_sensor(0)){ current_floor = 0; }
  else if(hardware_read_floor_sensor(1)){ current_floor = 1; }
  else if(hardware_read_floor_sensor(2)){ current_floor = 2; }
  else if(hardware_read_floor_sensor(3)){ current_floor = 3; }
  else{ above_or_below(above_flag, current_movement); }
  return current_floor;
}



HardwareMovement choose_init_direction(int UP_list[], int DOWN_list[], int current_floor, _Bool * wrong_dir_flag, _Bool above_flag){
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
      if(UP_list[i] == 1){
        if(i < current_floor){
          *wrong_dir_flag = 1;
          return HARDWARE_MOVEMENT_DOWN;
        } else if((i == current_floor) && (hardware_read_floor_sensor(i) == 0)){
          if(above_flag == 1){ return HARDWARE_MOVEMENT_DOWN; }
          else{ return HARDWARE_MOVEMENT_UP; }
        } else{
          return HARDWARE_MOVEMENT_UP;
      }
    }

    if(DOWN_list[i] == 1){
        if(i > current_floor){
          *wrong_dir_flag = 1;
          return HARDWARE_MOVEMENT_UP;
        } else if((i == current_floor) && (hardware_read_floor_sensor(i) == 0)){
          if(above_flag == 1){ return HARDWARE_MOVEMENT_DOWN; }
          else{ return HARDWARE_MOVEMENT_UP; }
        } else{
            return HARDWARE_MOVEMENT_DOWN;
        }
    }
  }
  return HARDWARE_MOVEMENT_STOP;
}




void check_higher_order(int DOWN_list[], int current_floor, _Bool * stop_flag_up){
  if(current_floor < 3){
    for(int i = (current_floor + 1); i < HARDWARE_NUMBER_OF_FLOORS; i++){
        if((DOWN_list[i] == 1)){
            *stop_flag_up = 0;
        }
    }
  }
  else if(current_floor == 3){
    *stop_flag_up = 1;
  }
}

void check_lower_order(int UP_list[], int current_floor, _Bool * stop_flag_down){
  if(current_floor > 0){
    for(int i = (current_floor-1); i >= 0; i--){
        if((UP_list[i] == 1)){
            *stop_flag_down = 0;
        }
    }
  }
  else if(current_floor == 0){
    *stop_flag_down = 1;
  }
}


void stop_UP_list_elevator(int UP_list[], int DOWN_list[], int current_floor, HardwareMovement *current_movement, _Bool *wrong_dir_flag, _Bool stop_flag_down, _Bool * stop_flag_up) {
  for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if((stop_flag_down == 1) && (UP_list[i] == 1) && (hardware_read_floor_sensor(i))){
      UP_list[i] = 0;
      DOWN_list[i] = 0;
      *wrong_dir_flag = 0;

      hardware_command_order_light(i, HARDWARE_ORDER_UP, 0);
      hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 0);
      hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 0);
      wait_3_seconds(UP_list, DOWN_list, current_floor, current_movement);
      for(int j = i; j<HARDWARE_NUMBER_OF_FLOORS; j++){
        if(UP_list[j] == 1){
        *current_movement = HARDWARE_MOVEMENT_UP;
        hardware_command_movement(*current_movement);
      }
  }
}
}
}
void stop_DOWN_list_elevator(int DOWN_list[], int UP_list[], int current_floor, HardwareMovement *current_movement, _Bool *wrong_dir_flag, _Bool stop_flag_up){
  for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if((stop_flag_up == 1) && (DOWN_list[i] == 1) && (hardware_read_floor_sensor(i))){
      DOWN_list[i] = 0;
      UP_list[i] = 0;
      *wrong_dir_flag = 0;
      hardware_command_order_light(i, HARDWARE_ORDER_UP, 0);
      hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 0);
      hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 0);
      wait_3_seconds(UP_list, DOWN_list, current_floor, current_movement);
    }
  }
}

void clear_all_orders(int UP_list[], int DOWN_list[]){
  for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    UP_list[i] = 0;
    DOWN_list[i] = 0;
  }
}

void update_elevator(int * current_floor,int UP_list[],int DOWN_list[], HardwareMovement movement, _Bool *above_flag){
  set_current_floor_light(*current_floor);
  set_order_lights();
  *current_floor = read_current_floor(*current_floor, above_flag, movement);


  set_UP_list(UP_list);
  set_DOWN_list(DOWN_list);
  set_INSIDE_order(UP_list, DOWN_list);
}
