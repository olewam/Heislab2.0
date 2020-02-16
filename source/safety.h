/**
  * @file
  * @brief safetyfunctions for the elevator
*/

/**
  * @brief stop the elevator if obstruction switch is used
*/
void read_obstruction_signal();

/**
  * @brief stop the elevator when button is pushed
*/
void stop_button_pushed();

/**
  * @brief Stop elevator
*/
void sigint_handler(int sig);
