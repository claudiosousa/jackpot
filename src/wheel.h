/**
 * Some helper functions for managing execution time.
 * @author Claudio Sousa, David Gonzalez
 */
#ifndef WHEEL_H
#define WHEEL_H

#include "game_state.h"

typedef struct wheel_t wheel_t;

wheel_t* wheel_start(game_t* game, int delay, int wheel_id, int* wheel_val);
void wheel_join(wheel_t* wheel);

#endif