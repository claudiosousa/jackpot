/**
 * Module handling the display
 * @author Claudio Sousa, David Gonzalez
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "game_state.h"

typedef struct display_t display_t;

display_t* display_start(game_t* game, game_data_t* gamedata);
void display_join(display_t* wheel);

#endif