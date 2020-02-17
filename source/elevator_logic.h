/**
  * @file
  * @brief functions to create the elevator logic
*/

/**
  * @brief default position for elevator on startup
  *
  * @return floor number for startup
*/
int find_default_floor();

/**
  * @brief find current floor
  *
  * @return current floor
*/
int current_floor(int temp_floor);

/**
  * @brief choose movement direction when stationary
  *
  * @return movement direction for first order
*/
HardwareMovement choose_init_direction(int UP_list[], int DOWN_list[], int current_floor);

/**
  * @brief stop elevator based on UP_list
  *
  * @return stop elevator
*/
HardwareMovement stop_UP_list_elevator(int UP_list[], int current_floor, HardwareMovement current_movement);

/**
  * @brief stop elevator based on DOWN_list[]
  *
  * @return stop elevator
*/
HardwareMovement stop_DOWN_list_elevator(int DOWN_list[], int current_floor, HardwareMovement current_movement);

/**
  * @brief clear all orders
*/
void clear_all_orders(int UP_list[], int DOWN_list[]);
