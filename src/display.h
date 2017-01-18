#ifndef DISPLAY_H
#define DISPLAY_H

#include "game_state.h"

typedef struct display_t display_t;

display_t* display_start(game_t* game, int* wheels, int wheelcount);
void display_join(display_t* wheel);

#endif