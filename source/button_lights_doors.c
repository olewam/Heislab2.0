/**
  * @file
  * @brief implementation of functions for buttons, lights and doors
*/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"

void set_current_floor_light(int temp_floor)
{
  hardware_command_floor_indicator_on(temp_floor);
}

void set_order_lights(){
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
