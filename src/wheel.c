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

static void* wheel_run(void* arg) {
    wheel_run_t* wheelp = (wheel_run_t*)arg;
    struct timespec ts;
    *wheelp->wheel_val = 0;

    timer_start(&ts);
    while (wheelp->game->state != GAME_STOP) {
        pthread_mutex_lock(&wheelp->game->state_m);
        while (wheelp->game->state != GAME_STOP &&
               (wheelp->game->stopped_wheels > wheelp->wheelnb || wheelp->game->state != GAME_RUNNING))
            pthread_cond_wait(&wheelp->game->state_change, &wheelp->game->state_m);
        pthread_mutex_unlock(&wheelp->game->state_m);

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
/*


BLOCK
sigset_t mask;
sigfillset(&mask);
pthread_sigmask(SIG_SETMASK, &mask, NULL);

SET HANDLE
struct sigaction act;
memset(&act, 0, sizeof(act));
act.sa_handler = handler;u
sigaction(SIGINT, &act, NULL);

UINLOCK
sigdelset(&mask, SIGINT);
pthread_sigmask(SIG_SETMASK, &mask, NULL);

//wait signal
sigwait



//WAIT
sigset_t mask, maskold;
sigfillset(&mask);
pthread_sigmask(SIG_SETMASK, &mask, &maskold);

sigwait(&mask, &sig);

//SIGALRM
unsigned int alarm(unsigned int seconds);
*/