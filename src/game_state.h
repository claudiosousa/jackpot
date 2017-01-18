#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <pthread.h>

typedef enum { GAME_WAITING_COIN, GAME_RUNNING, GAME_OVER, GAME_STOP } gamestate;

typedef struct {
    int stopped_wheels;
    gamestate state;
    pthread_cond_t state_change;
    pthread_mutex_t state_m;
} game_t;

void game_state_set(game_t* game, gamestate state);
void game_state_wait(game_t* game, gamestate state);

#endif
