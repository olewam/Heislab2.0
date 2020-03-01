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
void terminate_elevator();

/**
  * @brief stop the elevator when button is pushed, and start it again in correct direction
*/
void stop_button_pushed(HardwareMovement *current_movement, int current_floor, int UP_list[], int DOWN_list[], _Bool * wrong_dir_flag);
