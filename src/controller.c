#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "game_state.h"
#include "wheel.h"
#include "display.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

static gameresult controller_game_result(int *wheels) {
    int most_matched = 0;
    int matched;
    for (int i = 0; i < WHEEL_COUNT - 1; i++) {
        matched = 1;
        for (int j = i + 1; j < WHEEL_COUNT; j++)
            if (wheels[i] == wheels[j])
                matched++;

        if (matched > most_matched)
            most_matched = matched;
    }
    return most_matched >= 3 ? JACKPOT : (most_matched == 2 ? DOUBLE_WIN : LOST);
}

static void controller_play_loop(sigset_t *mask, game_t *game, game_data_t *gamedata) {
    int sig;
    do {
        sigwait(mask, &sig);  // wait SIGQUIT | SIGALRM | SIGTSTP | SIGINT

        pthread_mutex_lock(&game->state_m);

        if ((sig == SIGINT || sig == SIGALRM) && game->state == GAME_RUNNING) {
            game->stopped_wheels++;
            if (game->stopped_wheels == WHEEL_COUNT) {
                alarm(0);
                gamedata->result = controller_game_result(gamedata->wheels);
                gamedata->money_won = gamedata->result == JACKPOT
                                          ? gamedata->money_machine / 2
                                          : (gamedata->result == DOUBLE_WIN ? MIN(2, gamedata->money_machine) : 0);
                gamedata->money_machine -= gamedata->money_won;
                game->state = GAME_OVER;
            } else
                alarm(MAX_MOVE_TIME);
        } else if (sig == SIGTSTP) {
            alarm(MAX_MOVE_TIME);
            gamedata->money_machine++;
            game->stopped_wheels = 0;
            game->state = GAME_RUNNING;
        } else if (sig == SIGQUIT)
            game->state = GAME_STOP;

        pthread_cond_broadcast(&game->state_change);
        pthread_mutex_unlock(&game->state_m);

        if (game->state == GAME_OVER) {
            sleep(GAME_FINISHED_WAIT);
            pthread_mutex_lock(&game->state_m);
            game->state = GAME_WAITING_COIN;
            pthread_cond_broadcast(&game->state_change);
            pthread_mutex_unlock(&game->state_m);
        }
    } while (game->state != GAME_STOP);
}

static void *controller_play(void *data) {
    (void)data;

    game_t game;
    game.state = GAME_WAITING_COIN;

    game_data_t gamedata;
    gamedata.money_machine = INITIAL_MONEY;
    memset(gamedata.wheels, 0, sizeof(int) * WHEEL_COUNT);  // wheel initial state=0

    wheel_t *wheels[WHEEL_COUNT];

    int delay = WHEEL_BASE_DELAY_MS;
    for (int i = 0; i < WHEEL_COUNT; i++) {
        wheels[i] = wheel_start(&game, delay, i, gamedata.wheels + i);
        delay /= 2;  // could be replaced by an array of delays
    }

    display_t *display = display_start(&game, &gamedata);

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGQUIT & SIGALRM & SIGTSTP & SIGINT);
    pthread_sigmask(SIG_SETMASK, &mask, NULL);

    controller_play_loop(&mask, &game, &gamedata);

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
        perror("play_thread join");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
