#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <pthread.h>

#define WHEEL_COUNT 3
#define WHEEL_BASE_DELAY_MS 120
#define INITIAL_MONEY 10
#define MAX_MOVE_TIME 3
#define GAME_FINISHED_WAIT 5

typedef enum { GAME_WAITING_COIN, GAME_RUNNING, GAME_OVER, GAME_STOP } gamestate;
typedef enum { JACKPOT, DOUBLE_WIN, LOST } gameresult;

typedef struct {
    int stopped_wheels;
    gamestate state;
    pthread_cond_t state_change;
    pthread_mutex_t state_m;
} game_t;

typedef struct {
    int wheels[WHEEL_COUNT];
    int money_machine;
    int money_won;
    gameresult result;
} game_data_t;

#endif
