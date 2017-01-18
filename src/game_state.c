#include "game_state.h"

void game_state_wait(game_t* game, gamestate state) {
    pthread_mutex_lock(&game->state_m);
    while (game->state != state && game->state != GAME_STOP)
        pthread_cond_wait(&game->state_change, &game->state_m);
    pthread_mutex_unlock(&game->state_m);
}
