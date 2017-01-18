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
    int* wheel;
    game_t* game;
} wheel_run_t;

static void* wheel_run(void* arg) {
    wheel_run_t* wheelp = (wheel_run_t*)arg;
    struct timespec ts;
    *wheelp->wheel = 0;

    timer_start(&ts);
    do {
        (*wheelp->wheel)++;
        *wheelp->wheel %= WHEEL_VALUES;
        timer_wait(&ts, wheelp->delay);
    } while (wheelp->game->state != GAME_STOP);

    free(arg);
    return NULL;
}

wheel_t* wheel_start(game_t* game, int delay, int* wheelv) {
    wheel_t* wheel = malloc(sizeof(wheel_t));

    wheel_run_t* wheep = malloc(sizeof(wheel_run_t));
    wheep->delay = delay;
    wheep->wheel = wheelv;
    wheep->game = game;

    if (pthread_create(&wheel->thread, NULL, wheel_run, wheep) != 0) {
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