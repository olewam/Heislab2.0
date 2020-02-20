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

int current_floor(int temp_floor)
{
  if(hardware_read_floor_sensor(1)){ temp_floor = 1; }
  if(hardware_read_floor_sensor(2)){ temp_floor = 2; }
  if(hardware_read_floor_sensor(3)){ temp_floor = 3; }
  if(hardware_read_floor_sensor(0)){ temp_floor = 0; }
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
      /*if(current_floor >= f){
        DOWN_list[f] = 1;
      }
      if(current_floor <= f){
        UP_list[f] = 1;
      }*/
      if(f != 3){
        UP_list[f] = 1;
      }
      if(f != 0){
        DOWN_list[f] = 1;
      }

    }
  }
}

HardwareMovement choose_init_direction(int UP_list[], int DOWN_list[], int temp_floor, _Bool * wrong_dir_flag){
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
      if(UP_list[i] == 1){
        if(i < temp_floor){
          *wrong_dir_flag = 1;
          return HARDWARE_MOVEMENT_DOWN;
          /*while(i != temp_floor){
              hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
              set_UP_list(UP_list);
              set_DOWN_list(DOWN_list);
              stop_DOWN_list_elevator()
              temp_floor = current_floor(temp_floor);
          }*/
        } else {
          return HARDWARE_MOVEMENT_UP;
      }
    }

    if(DOWN_list[i] == 1){
        if(i > temp_floor){
          *wrong_dir_flag = 1;
          return HARDWARE_MOVEMENT_UP;
          /*while(i != temp_floor){UP_list[i] == 1)
              hardware_command_movement(HARDWARE_MOVEMENT_UP);
              set_UP_list(UP_list);
              set_DOWN_list(DOWN_list);
              temp_floor = current_floor(temp_floor);
          }*/
        } else {
            return HARDWARE_MOVEMENT_DOWN;
        }
    }
  }
  return HARDWARE_MOVEMENT_STOP;
}

/*
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
*/


void wait_3_seconds(int UP_list[], int DOWN_list[], int current_floor, HardwareMovement *current_movement){
  double initTime = clock();

  set_current_floor_light(current_floor);
  hardware_command_door_open(1);
  while(1){
    *current_movement = HARDWARE_MOVEMENT_STOP;
    hardware_command_movement(*current_movement);

    set_order_lights();
    set_UP_list(UP_list);
    set_DOWN_list(DOWN_list);
    handle_inside_order(UP_list, DOWN_list);

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


void stop_UP_list_elevator(int UP_list[], int DOWN_list[], int current_floor, HardwareMovement *current_movement, _Bool *wrong_dir_flag, _Bool stop_flag_down) {
  //printf("%d\n", stop_flag_down);
  for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if((stop_flag_down == 1) && (UP_list[i] == 1) && (hardware_read_floor_sensor(i))){
      UP_list[i] = 0;
      DOWN_list[i] = 0;
      *wrong_dir_flag = 0;

      hardware_command_order_light(i, HARDWARE_ORDER_UP, 0);
      hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 0);
      hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 0);
      wait_3_seconds(UP_list, DOWN_list, current_floor, current_movement);
    }
  }
}

void stop_DOWN_list_elevator(int DOWN_list[], int UP_list[], int current_floor, HardwareMovement *current_movement, _Bool *wrong_dir_flag, _Bool stop_flag_up){
  //printf("%d\n", stop_flag_up);
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
