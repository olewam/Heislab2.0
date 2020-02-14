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

/*
void set_buttonlights_outside()

void set_buttonlights_inside()

void door_open_lights() //3 sek
*/
