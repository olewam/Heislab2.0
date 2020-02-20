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
void handle_inside_order(int UP_list[], int DOWN_list[]);

/**
  * @brief choose movement direction when stationary
  *
  * @return movement direction for first order
*/
HardwareMovement choose_init_direction(int UP_list[], int DOWN_list[], int temp_floor, _Bool * wrong_dir_flag);

/**
  * @brief stop for 3 sek
*/
void wait_3_seconds(int UP_list[], int DOWN_list[], int current_floor, HardwareMovement *current_movement);

/**
  * @brief check if there is an order higher up that the elevator need to handle first
*/
void check_higher_order(int DOWN_list[], int current_floor, _Bool * stop_flag_up);

/**
  * @brief check if there is an order lower that the elevator need to handle first
*/
void check_lower_order(int UP_list[], int current_floor, _Bool * stop_flag_down);

/**
  * @brief stop elevator based on UP_list
  *
  * @return stop elevator
*/
HardwareMovement stop_UP_list_elevator(int UP_list[], int DOWN_list[], int current_floor, HardwareMovement *current_movement, _Bool *wrong_dir_flag, _Bool stop_flag_down);

/**
  * @brief stop elevator based on DOWN_list[]
  *
  * @return stop elevator
*/
HardwareMovement stop_DOWN_list_elevator(int DOWN_list[], int UP_list[], int current_floor, HardwareMovement *current_movement, _Bool *wrong_dir_flag, _Bool stop_flag_up);

/**
  * @brief clear all orders
*/
void clear_all_orders(int UP_list[], int DOWN_list[]);
