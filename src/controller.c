#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "wheel.h"
#include "display.h"

#define WHEEL_COUNT 3
#define WHEEL_DELAY_MS 120

volatile sig_atomic_t etst;

static void *controller_play(void *data) {
    (void)data;

    wheel_t *wheels[WHEEL_COUNT];
    int wheel_values[WHEEL_COUNT];

    int delay = WHEEL_DELAY_MS;
    for (int i = 0; i < WHEEL_COUNT; i++) {
        wheel_values[i] = 0;
        wheels[i] = wheel_start(delay, wheel_values + i);
        delay /= 2;  // could be replaced by an array of delays
    }

    display_t *display = display_start(wheel_values, WHEEL_COUNT);

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGQUIT & SIGALRM & SIGTSTP & SIGINT);
    pthread_sigmask(SIG_SETMASK, &mask, NULL);

    int sig;
    do {
        sigwait(&mask, &sig);
        if (sig == SIGINT)
            printf("COIN\n");
        else if (sig == SIGTSTP)
            printf("WHEEL\n");
        else if (sig == SIGALRM)
            printf("ALARM\n");
    } while (sig != SIGQUIT);

    printf("Quiting..\n");

    display_stop(display);

    for (int i = 0; i < WHEEL_COUNT; i++)
        wheel_stop(wheels[i]);

    return NULL;
}

int controller_start() {
    sigset_t mask;
    sigfillset(&mask);
    pthread_sigmask(SIG_SETMASK, &mask, NULL);

    pthread_t play_thread;

    if (pthread_create(&play_thread, NULL, controller_play, NULL) != 0) {
        perror("play_thread create");
        return EXIT_FAILURE;
    }

    if (pthread_join(play_thread, NULL) != 0) {
        perror("play_thread create");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
