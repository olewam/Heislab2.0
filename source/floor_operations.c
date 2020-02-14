#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"

int current_floor(int temp_floor)
{
  if(hardware_read_floor_sensor(1)){ temp_floor = 1; }
  if(hardware_read_floor_sensor(2)){ temp_floor = 2; }
  if(hardware_read_floor_sensor(3)){ temp_floor = 3; }
  if(hardware_read_floor_sensor(0)){ temp_floor = 0; }
  return temp_floor;
}

void set_current_floor_light(int temp_floor)
{
  hardware_command_floor_indicator_on(temp_floor);
}
