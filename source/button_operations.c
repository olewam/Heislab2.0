/**
  * @file
  * @brief implementation of button operation functions
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"


void stop_button_pushed(HardwareMovement current_movement){
  while(hardware_read_stop_signal()){
      hardware_command_stop_light(1);
      hardware_command_movement(HARDWARE_MOVEMENT_STOP);
  }
  hardware_command_movement(current_movement);  //fjern senere, når bestillinger er lagt inn
  hardware_command_stop_light(0);
}

void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

/*
void set_buttonlights_outside()

void set_buttonlights_inside()

void door_open_lights() //3 sek


void recieve_order(){
  int orders[10];
  for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
      for(int i = 0; i < 3; i++){
          int orders_floor = f;
          HardwareMovement order_type = hardware_order_type_bit(i);
          if(hardware_read_order(order_floor,order_type)){
            for(int i = 0; i < 10; i++){
              if(orders[i] != '\0'){
                orders[i] = orders_floor;
              }
            }
          }
      }
  }
  for(int i = 0; i < 10; i++){
    printf("%d\n", orders[i]);
  }
}
*/
