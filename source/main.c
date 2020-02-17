/**
  * @file
  * @brief main file for running the elevator code
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "button_lights_doors.h"
#include "order_functions.h"
#include "safety.h"
#include "elevator_logic.h"
#include <time.h>
#include <unistd.h>

int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);

	  int floor = find_default_floor();
    printf("%d\n", floor);
	  HardwareMovement movement = HARDWARE_MOVEMENT_STOP;
	  hardware_command_movement(movement);

    int UP_list[] = {0, 0, 0};
    int DOWN_list[] = {0, 0, 0};

    while(1){
      read_obstruction_signal();
      movement = elevator_limits(movement);
      set_current_floor_light(floor);
      set_order_lights();
      floor = current_floor(floor);

      stop_button_pushed(&movement, UP_list, DOWN_list);

      set_UP_list(UP_list);
      set_DOWN_list(DOWN_list);
      //handle_inside_order(floor, UP_list, DOWN_list);

      switch(movement){
        case HARDWARE_MOVEMENT_UP:
            movement = stop_UP_list_elevator(UP_list, floor, movement);
            movement = stop_DOWN_list_elevator(DOWN_list, floor, movement);
            hardware_command_movement(movement);
            /*for(int i = 0; i < size; i++){
              if((UP_list[i] == 1) && (i == floor)){
                movement = HARDWARE_MOVEMENT_STOP;
                hardware_command_movement(movement);
                UP_list[i] = 0;

                for(int j = (i+1); j < size ; j++){
                  if(UP_list[j] == 1){
                    movement = HARDWARE_MOVEMENT_UP;
                    hardware_command_movement(movement);
                  }
                }
              }
            }*/
            break;

          case HARDWARE_MOVEMENT_DOWN:
              movement = stop_DOWN_list_elevator(DOWN_list, floor, movement);
              movement = stop_UP_list_elevator(UP_list, floor, movement);
              hardware_command_movement(movement);

              /*for(int i = 0; i < size; i++){
                if((DOWN_list[i] == 1) && ((i + 1) == floor)){
                  movement = HARDWARE_MOVEMENT_STOP;
                  hardware_command_movement(movement);
                  DOWN_list[i] = 0;
                  for(int j = (i-1); j >= 0; j--){
                    if(DOWN_list[j] == 1){
                      movement = HARDWARE_MOVEMENT_DOWN;
                      hardware_command_movement(movement);
                    }
                  }
                }
              }*/
              break;

          case HARDWARE_MOVEMENT_STOP:
              movement = choose_init_direction(UP_list, DOWN_list, floor);
              hardware_command_movement(movement);
              break;

          default:
              printf("Hello suckers");
              break;
      }
    }
    return 0;
}
