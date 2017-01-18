#ifndef WHEEL_H
#define WHEEL_H

#include "game_state.h"

typedef struct wheel_t wheel_t;

wheel_t* wheel_start(game_t* game, int delay, int wheelnb, int* wheel_val);
void wheel_join(wheel_t* wheel);

#endif