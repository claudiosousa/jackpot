#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    game_state_wait(displayp->game, GAME_RUNNING);
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
}

static void display_game_over() {
    setcursor(5, 1);
    printf("RESULTAT ....................!");
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
            case GAME_OVER:
                display_game_over();
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