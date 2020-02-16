/**
  * @file
  * @brief functions for floor operations
*/


/**
  * @brief return current floor
*/
int current_floor(int temp_floor);

/**
  * @brief turn on the lights for current floor
*/
void set_current_floor_light(int temp_floor);

/**
  * @brief set and clear the order lights on the panel
*/
void set_and_clear_order_lights();

/**
  * @brief clear all the order lights at once
*/
void clear_all_order_lights();

/**
  * @brief choose movement direction when stationary
*/
HardwareMovement choose_init_direction(int UP_list[], int DOWN_list[]);
