/**
  * @file
  * @brief main file for running the elevator code.
  * while loop handling movement of elevator
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "button_lights_doors.h"
#include "safety.h"
#include "elevator_logic.h"
#include <time.h>
#include <unistd.h>
#include <stdbool.h>


int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);

	  int floor = find_default_floor();
	  HardwareMovement movement = HARDWARE_MOVEMENT_STOP;

    int UP_list[] = {0, 0, 0, 0};
    int DOWN_list[] = {0, 0, 0, 0};

    _Bool stop_flag_up = 1;
    _Bool stop_flag_down = 1;
    _Bool wrong_dir_flag = 0;
    _Bool above_flag = 0;


    while(1){
      terminate_elevator();
      stop_button_pushed(&movement, floor, UP_list, DOWN_list, &wrong_dir_flag);

      update_elevator(&floor, UP_list, DOWN_list, movement, &above_flag);

      switch(movement){
        case HARDWARE_MOVEMENT_UP:
            if(wrong_dir_flag == 0){
              stop_UP_list_elevator(UP_list, DOWN_list, floor, &movement, &wrong_dir_flag, stop_flag_down);
            }
            if(wrong_dir_flag == 1){
                check_higher_order(DOWN_list, floor, &stop_flag_up);
                stop_DOWN_list_elevator(DOWN_list, UP_list, floor, &movement, &wrong_dir_flag, stop_flag_up);
                stop_UP_list_elevator(UP_list, DOWN_list, floor, &movement, &wrong_dir_flag, stop_flag_down);
                stop_flag_up = 1;
            }
            break;

          case HARDWARE_MOVEMENT_DOWN:
              if(wrong_dir_flag == 0){
                stop_DOWN_list_elevator(DOWN_list, UP_list, floor, &movement, &wrong_dir_flag, stop_flag_up);
              }
              if(wrong_dir_flag == 1){
                    check_lower_order(UP_list, floor, &stop_flag_down);
                    stop_UP_list_elevator(UP_list, DOWN_list, floor, &movement, &wrong_dir_flag, stop_flag_down);
                    stop_DOWN_list_elevator(DOWN_list, UP_list, floor, &movement, &wrong_dir_flag, stop_flag_up);
                    stop_flag_down = 1;
              }
              break;

          case HARDWARE_MOVEMENT_STOP:
              movement = choose_init_direction(UP_list, DOWN_list, floor, &wrong_dir_flag, above_flag);
              hardware_command_movement(movement);
              break;

          default:
              printf("Sorry Kolbjørn, we failed you");
              break;
      }
    }
    return 0;
}
