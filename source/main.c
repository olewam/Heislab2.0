#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "floor_operations.h"
#include "button_operations.h"
#include "order_functions.h"
#include <time.h>
#include <unistd.h>

int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }


    signal(SIGINT, sigint_handler);

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

	  int floor;
	  HardwareMovement movement = HARDWARE_MOVEMENT_STOP;

	  hardware_command_movement(movement);

    int size = 3;
    int UP_list[] = {0, 0, 0};
    int DOWN_list[] = {0, 0, 0};

    while(1){
      //printf("%d\n", hardware_read_order( 2 ,HARDWARE_ORDER_UP));

      set_current_floor_light(floor);

      floor = current_floor(floor);

      //set_orders(floor);
      //printf("%d\n", floor);

      stop_button_pushed(movement);

      set_UP_list(UP_list);
      set_DOWN_list(DOWN_list);
      //handle_inside_order(floor, UP_list, DOWN_list);

      switch(movement){
        case HARDWARE_MOVEMENT_UP:
            for(int i = 0; i < size; i++){
              if((UP_list[i] == 1) && (i == floor)){
                movement = HARDWARE_MOVEMENT_STOP;
                hardware_command_movement(movement);
                UP_list[i] = 0;
                /*
                for(int j = (i+1); j < size ; j++){
                  if(UP_list[j] == 1){
                    movement = HARDWARE_MOVEMENT_UP;
                    hardware_command_movement(movement);
                  }
                }*/
              }
            }
            break;

          case HARDWARE_MOVEMENT_DOWN:
              for(int i = 0; i < size; i++){
                if((DOWN_list[i] == 1) && ((i + 1) == floor)){
                  movement = HARDWARE_MOVEMENT_STOP;
                  hardware_command_movement(movement);
                  DOWN_list[i] = 0;
                  /*
                  for(int j = (i-1); j >= 0; j--){
                    if(DOWN_list[j] == 1){
                      movement = HARDWARE_MOVEMENT_DOWN;
                      hardware_command_movement(movement);
                    }
                  }*/
                }
              }
              break;

          case HARDWARE_MOVEMENT_STOP:
              movement = choose_init_direction(UP_list, DOWN_list);
              hardware_command_movement(movement);
              break;

          default:
              printf("Hello suckers");
              break;
      }




        /*if(hardware_read_stop_signal()){
            hardware_command_movement(movement);
            break;
        }
	*/

  /*  Få heisen til å snu i endepunktene
        if(hardware_read_floor_sensor(0)){
            movement = HARDWARE_MOVEMENT_UP;
            hardware_command_movement(movement);
        }
        if(hardware_read_floor_sensor(HARDWARE_NUMBER_OF_FLOORS - 1)){
           movement = HARDWARE_MOVEMENT_DOWN;
           hardware_command_movement(movement);
        }

        */

       //All buttons must be polled, like this:
       /**
       for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
                hardware_command_floor_indicator_on(f);
            }
        }
        **/

        // Lights are set and cleared like this:
         for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            // Internal orders
            if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
                hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
            }

            // Orders going up
            if(hardware_read_order(f, HARDWARE_ORDER_UP)){
                hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
            }

            // Orders going down
            if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
                hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
            }
        }


        if(hardware_read_obstruction_signal()){
            hardware_command_stop_light(1);
            clear_all_order_lights();
            sigint_handler(1);
        }
        else{
            hardware_command_stop_light(0);
        }
    }
    return 0;
}
