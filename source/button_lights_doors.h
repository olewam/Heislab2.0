/**
  * @file
  * @brief functions for buttons, lights and doors
*/

/**
  * @brief turn on the lights for current floor
*/
void set_current_floor_light(int current_floor);

/**
  * @brief set the order lights on the panel
*/
void set_order_lights();

/**
  * @brief set list of orders going UP
*/
void set_UP_list(int UP_list[]);

/**
  * @brief set list of orders going DOWN
*/
void set_DOWN_list(int DOWN_list[]);

/**
  * @brief handle orders from inside elavtor
*/
void set_INSIDE_order(int UP_list[], int DOWN_list[]);


/**
  * @brief stop for 3 sec
*/
void wait_3_seconds(int UP_list[], int DOWN_list[], int current_floor, HardwareMovement *current_movement);

/**
  * @brief clear all the order lights at once
*/
void clear_all_order_lights();
