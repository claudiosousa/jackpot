#include "game_state.h"

void game_state_set(game_t* game, gamestate state) {
    pthread_mutex_lock(&game->state_m);
    game->state = state;
    pthread_cond_broadcast(&game->state_change);
    pthread_mutex_unlock(&game->state_m);
}

void game_state_wait(game_t* game, gamestate state) {
    pthread_mutex_lock(&game->state_m);
    while (game->state != state && game->state != GAME_STOP)
        pthread_cond_wait(&game->state_change, &game->state_m);
    pthread_mutex_unlock(&game->state_m);
}
