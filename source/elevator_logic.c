/**
  * @file
  * @brief implementation of functions to create the elevator logic
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "button_lights_doors.h"
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
  return 0; //Never happens
}

void above_or_below(_Bool *above_flag, HardwareMovement current_movement){
  if(current_movement == HARDWARE_MOVEMENT_UP){
    *above_flag = 1;
  } else if (current_movement == HARDWARE_MOVEMENT_DOWN){
    *above_flag = 0;
  }
}

int current_floor(int temp_floor, _Bool *above_flag, HardwareMovement current_movement)
{
  if(hardware_read_floor_sensor(1)){ temp_floor = 1; }
  if(hardware_read_floor_sensor(2)){ temp_floor = 2; }
  if(hardware_read_floor_sensor(3)){ temp_floor = 3; }
  if(hardware_read_floor_sensor(0)){ temp_floor = 0; }
  else{ above_or_below(above_flag, current_movement); }
  return temp_floor;
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

void handle_inside_order(int UP_list[], int DOWN_list[]){
  for(int f = 0; f < (HARDWARE_NUMBER_OF_FLOORS); f++){
    if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
      /*if(f != 3){
        UP_list[f] = 1;
      }
      if(f != 0){
        DOWN_list[f] = 1;
      }*/
      UP_list[f] = 1;
      DOWN_list[f] = 1;
    }
  }
}

HardwareMovement choose_init_direction(int UP_list[], int DOWN_list[], int temp_floor, _Bool * wrong_dir_flag, _Bool above_flag){
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
      if(UP_list[i] == 1){
        if(i < temp_floor){
          *wrong_dir_flag = 1;
          return HARDWARE_MOVEMENT_DOWN;
        } else if((i == temp_floor) && (hardware_read_floor_sensor(i) == 0)){
          if(above_flag == 1){ return HARDWARE_MOVEMENT_DOWN; }
          else{ return HARDWARE_MOVEMENT_UP; }
        } else{
          return HARDWARE_MOVEMENT_UP;
      }
    }

    if(DOWN_list[i] == 1){
        if(i > temp_floor){
          *wrong_dir_flag = 1;
          return HARDWARE_MOVEMENT_UP;
        } else if((i == temp_floor) && (hardware_read_floor_sensor(i) == 0)){
          if(above_flag == 1){ return HARDWARE_MOVEMENT_DOWN; }
          else{ return HARDWARE_MOVEMENT_UP; }
        } else{
            return HARDWARE_MOVEMENT_DOWN;
        }
    }
  }
  return HARDWARE_MOVEMENT_STOP;
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
    handle_inside_order(UP_list, DOWN_list);
    while(hardware_read_obstruction_signal()){
      obst_flag = 1;
      continue;
    }
    if(obst_flag == 1){
      wait_3_seconds(UP_list, DOWN_list, current_floor, current_movement);
    }
    // Sjekk om noe er over eller under basert på state, hvis ikke, stop.


    if((clock() - initTime)/CLOCKS_PER_SEC >= 3){
      hardware_command_door_open(0);
      break;
    }
  }
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
