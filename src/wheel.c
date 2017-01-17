#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "wheel.h"

struct wheel_t {
    bool stop;
    pthread_t thread;
};

typedef struct {
    int delay;
    int* wheel;
    bool* stop;
} wheel_run_t;

void* wheel_run(void* wheelp) {
    (void)wheelp;
    return NULL;
}

wheel_t* wheel_start(int delay, int* wheelv) {
    wheel_t* wheel = malloc(sizeof(wheel_t));
    wheel->stop = false;
    wheel_run_t wheel_run_p = {delay, wheelv, &wheel->stop};

    if (pthread_create(&wheel->thread, NULL, wheel_run, &wheel_run_p) != 0) {
        perror("play_thread create");
        return NULL;
    }

    if (pthread_join(wheel->thread, NULL) != 0) {
        perror("play_thread create");
        return NULL;
    }

    return wheel;
}

void wheel_stop(wheel_t* wheel) {
    (void)wheel;
}

void wheel_join(wheel_t* wheel) {
    (void)wheel;
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