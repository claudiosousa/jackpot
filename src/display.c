#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "display.h"
#include "timer.h"

#define REFRESH_INTERVAL 1000 / 30
#define GAME_FINISHED_WAIT 5

struct display_t {
    pthread_t thread;
};

typedef struct {
    game_data_t* game_data;
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
    while (displayp->game->state == GAME_WAITING_COIN)
        pthread_cond_wait(&displayp->game->state_change, &displayp->game->state_m);
    pthread_mutex_unlock(&displayp->game->state_m);
}

static void display_game_running_frame(display_run_t* displayp) {
    eraseline();
    for (int i = 0; i < WHEEL_COUNT; i++) {
        setcursor(3, i * 2 + 1);
        printf("%d", displayp->game_data->wheels[i]);
    }
}

static void display_game_running(display_run_t* displayp) {
    clearscren();
    setcursor(1, 1);
    printf("Game started!");
    setcursor(3, 1);

    struct timespec ts;
    timer_start(&ts);

    do {
        display_game_running_frame(displayp);
        timer_wait(&ts, REFRESH_INTERVAL);

    } while (displayp->game->state == GAME_RUNNING);

    pthread_mutex_lock(&displayp->game->state_m);
    display_game_running_frame(displayp);
    pthread_mutex_unlock(&displayp->game->state_m);

    if (displayp->game->state == GAME_WAITING_COIN) {
        setcursor(1, 1);
        printf("Game finished!");

        setcursor(5, 1);
        printf(displayp->game_data->result == JACKPOT
                   ? "JACKPOT!"
                   : (displayp->game_data->result == DOUBLE_WIN ? "Double win!" : "You lost!"));

        setcursor(6, 1);
        printf("You won %d coin%s", displayp->game_data->money_won, displayp->game_data->money_won != 1 ? "s" : "");
        setcursor(7, 1);
        printf("%d coin%s left in the machine", displayp->game_data->money_machine,
               displayp->game_data->money_machine != 1 ? "s" : "");

        sleep(GAME_FINISHED_WAIT);  // display behavior
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

display_t* display_start(game_t* game, game_data_t* game_data) {
    display_t* display = malloc(sizeof(display_t));

    display_run_t* displayp = malloc(sizeof(display_run_t));
    displayp->game_data = game_data;
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