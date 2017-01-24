#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "game_state.h"
#include "wheel.h"
#include "display.h"

#define WHEEL_COUNT 3
#define WHEEL_BASE_DELAY_MS 120

static void *controller_play(void *data) {
    (void)data;

    game_t game;
    game.state = GAME_WAITING_COIN;

    wheel_t *wheels[WHEEL_COUNT];
    int wheel_values[WHEEL_COUNT];
    memset(wheel_values, 0, sizeof(int) * WHEEL_COUNT);  // wheel initial state=0

    int delay = WHEEL_BASE_DELAY_MS;
    for (int i = 0; i < WHEEL_COUNT; i++) {
        wheels[i] = wheel_start(&game, delay, i, wheel_values + i);
        delay /= 2;  // could be replaced by an array of delays
    }

    display_t *display = display_start(&game, wheel_values, WHEEL_COUNT);

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGQUIT & SIGALRM & SIGTSTP & SIGINT);
    pthread_sigmask(SIG_SETMASK, &mask, NULL);

    int sig;
    do {
        sigwait(&mask, &sig);  // wait SIGQUIT | SIGALRM | SIGTSTP | SIGINT

        pthread_mutex_lock(&game.state_m);

        if ((sig == SIGINT && game.state == GAME_RUNNING) || sig == SIGALRM) {
            game.stopped_wheels++;
            if (game.stopped_wheels == WHEEL_COUNT)
                game.state = GAME_WAITING_COIN;
        } else if (sig == SIGTSTP) {
            game.stopped_wheels = 0;
            game.state = GAME_RUNNING;
        } else if (sig == SIGQUIT)
            game.state = GAME_STOP;

        pthread_cond_broadcast(&game.state_change);
        pthread_mutex_unlock(&game.state_m);
    } while (game.state != GAME_STOP);

    display_join(display);

    for (int i = 0; i < WHEEL_COUNT; i++)
        wheel_join(wheels[i]);

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
