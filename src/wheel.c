#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wheel.h"
#include "timer.h"

#define WHEEL_VALUES 10

struct wheel_t {
    pthread_t thread;
};

typedef struct {
    int delay;
    int wheelnb;
    int* wheel_val;
    game_t* game;
} wheel_run_t;

/** Checks if the current wheel should be paused.
    Pause if the game has not ended and not running or running but the wheel has been stoped already

    @param wheelp The wheel information
    @returns If the wheel should not update it's value
*/
static bool wheel_paused(wheel_run_t* wheelp) {
    return wheelp->game->state != GAME_STOP &&
           (wheelp->game->stopped_wheels > wheelp->wheelnb || wheelp->game->state != GAME_RUNNING);
}

static void* wheel_run(void* arg) {
    wheel_run_t* wheelp = (wheel_run_t*)arg;
    struct timespec ts;
    *wheelp->wheel_val = 0;

    timer_start(&ts);
    while (true) {
        if (wheel_paused(wheelp)) {  // we test wheel_paused twice to avoid unecessary locking
            pthread_mutex_lock(&wheelp->game->state_m);
            while (wheel_paused(wheelp))
                pthread_cond_wait(&wheelp->game->state_change, &wheelp->game->state_m);
            pthread_mutex_unlock(&wheelp->game->state_m);
        }
        if (wheelp->game->state == GAME_STOP)
            break;

        (*wheelp->wheel_val)++;
        *wheelp->wheel_val %= WHEEL_VALUES;
        timer_wait(&ts, wheelp->delay);
    }

    free(arg);
    return NULL;
}

wheel_t* wheel_start(game_t* game, int delay, int wheelnb, int* wheel_val) {
    wheel_t* wheel = malloc(sizeof(wheel_t));

    wheel_run_t* wheelp = malloc(sizeof(wheel_run_t));
    wheelp->delay = delay;
    wheelp->wheelnb = wheelnb;
    wheelp->wheel_val = wheel_val;
    wheelp->game = game;

    if (pthread_create(&wheel->thread, NULL, wheel_run, wheelp) != 0) {
        perror("wheel_thread create");
        return NULL;
    }

    return wheel;
}

void wheel_join(wheel_t* wheel) {
    if (pthread_join(wheel->thread, NULL) != 0)
        perror("wheel_thread join");

    free(wheel);
}