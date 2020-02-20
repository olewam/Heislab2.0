/**
  * @file
  * @brief safetyfunctions for the elevator
*/

/**
  * @brief Stop elevator
*/
void sigint_handler(int sig);

/**
  * @brief stop the elevator if obstruction switch is used
*/
void read_obstruction_signal();

/**
  * @brief stop the elevator when button is pushed
*/
void stop_button_pushed(HardwareMovement current_movement, int current_floor, int UP_list[], int DOWN_list[], _Bool * wrong_dir_flag);

/**
  * @brief stop elevator to go outside physical limits
  *
  *  @return stop elevator in end points
*/
HardwareMovement elevator_limits(HardwareMovement current_movement);
