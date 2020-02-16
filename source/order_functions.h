/**
  * @file
  * @brief functions for orders
*/

/**
  * @brief set list of orders going UP
*/
void set_UP_list(int UP_list[]);

/**
  * @brief set list of orders going DOWN
*/
void set_DOWN_list(int DOWN_list[]);

void handle_inside_order(int current_floor, int UP_list[], int DOWN_list[]);
