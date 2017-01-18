#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wheel.h"
#include "timer.h"

#define WHEEL_VALUES 10

struct wheel_t {
    bool stop;
    pthread_t thread;
};

typedef struct {
    int delay;
    int* wheel;
    bool* stop;
} wheel_run_t;

void* wheel_run(void* arg) {
    wheel_run_t* wheelp = (wheel_run_t*)arg;
    struct timespec ts;
    *wheelp->wheel = 0;

    timer_start(&ts);
    do {
        (*wheelp->wheel)++;
        *wheelp->wheel %= WHEEL_VALUES;
        timer_wait(&ts, wheelp->delay);
    } while (!*wheelp->stop);

    free(arg);
    return NULL;
}

wheel_t* wheel_start(int delay, int* wheelv) {
    wheel_t* wheel = malloc(sizeof(wheel_t));
    wheel->stop = false;

    wheel_run_t* wheep = malloc(sizeof(wheel_run_t));
    wheep->delay = delay;
    wheep->wheel = wheelv;
    wheep->stop = &wheel->stop;

    if (pthread_create(&wheel->thread, NULL, wheel_run, wheep) != 0) {
        perror("wheel_thread create");
        return NULL;
    }

    return wheel;
}

void wheel_stop(wheel_t* wheel) {
    wheel->stop = true;

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