#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "display.h"
#include "timer.h"

#define REFRESH_INTERVAL 1000 / 30

struct display_t {
    pthread_t thread;
};

typedef struct {
    int* wheels;
    int wheelcount;
    game_t* game;
} display_run_t;

static void hidecursor() {
    printf("\e[?25l");
}

static void clearscren() {
    printf("\e[2J");
}

static void setcursor(int y, int x) {
    printf("\e[%d;%dH", y, x);
}

static void eraseline() {
    printf("\e[2K");
}

static void display_waiting_coin(display_run_t* displayp) {
    clearscren();
    setcursor(1, 1);
    printf("Insert a coin to start the game...");

    pthread_mutex_lock(&displayp->game->state_m);
    while (displayp->game->state != GAME_STOP && displayp->game->state != GAME_RUNNING)
        pthread_cond_wait(&displayp->game->state_change, &displayp->game->state_m);
    pthread_mutex_unlock(&displayp->game->state_m);
}

static void display_game_running(display_run_t* displayp) {
    clearscren();
    setcursor(1, 1);
    printf("Game started!");
    setcursor(3, 1);

    struct timespec ts;
    timer_start(&ts);

    do {
        eraseline();
        for (int i = 0; i < displayp->wheelcount; i++) {
            setcursor(3, i * 2 + 1);
            printf("%d", displayp->wheels[i]);
        }
        timer_wait(&ts, REFRESH_INTERVAL);
    } while (displayp->game->state == GAME_RUNNING);

    if (displayp->game->state == GAME_WAITING_COIN) {
        setcursor(5, 1);
        printf("RESULTAT ....................!");
        sleep(5);
    }
}

static void display_game_stop() {
    clearscren();
    setcursor(1, 1);
    printf("Come again soon!\n");
}

static void* display_run(void* arg) {
    hidecursor();

    display_run_t* displayp = (display_run_t*)arg;
    do {
        switch (displayp->game->state) {
            case GAME_WAITING_COIN:
                display_waiting_coin(displayp);
                break;
            case GAME_RUNNING:
                display_game_running(displayp);
                break;
            default:
                break;
        }
    } while (displayp->game->state != GAME_STOP);

    display_game_stop();

    free(arg);
    return NULL;
}

display_t* display_start(game_t* game, int* wheels, int wheelcount) {
    display_t* display = malloc(sizeof(display_t));

    display_run_t* displayp = malloc(sizeof(display_run_t));
    displayp->wheels = wheels;
    displayp->wheelcount = wheelcount;
    displayp->game = game;

    if (pthread_create(&display->thread, NULL, display_run, displayp) != 0) {
        perror("play_thread create");
        return NULL;
    }

    return display;
}

void display_join(display_t* display) {
    if (pthread_join(display->thread, NULL) != 0)
        perror("display_thread join");

    free(display);
}