#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "display.h"
#include "timer.h"

#define REFRESH_INTERVAL 1000 / 30

struct display_t {
    bool stop;
    pthread_t thread;
};

typedef struct {
    int* wheels;
    int wheelcount;
    bool* stop;
} display_run_t;

void* display_run(void* arg) {
    display_run_t* displayp = (display_run_t*)arg;
    struct timespec ts;

    timer_start(&ts);
    do {
        // todo
        for (int i = 0; i < displayp->wheelcount; i++)
            printf("%d ", displayp->wheels[i]);
        printf("\n");

        timer_wait(&ts, REFRESH_INTERVAL);
    } while (!*displayp->stop);

    free(arg);
    return NULL;
}

display_t* display_start(int* wheels, int wheelcount) {
    display_t* display = malloc(sizeof(display_t));
    display->stop = false;

    display_run_t* displayp = malloc(sizeof(display_run_t));
    displayp->wheels = wheels;
    displayp->wheelcount = wheelcount;
    displayp->stop = &display->stop;

    if (pthread_create(&display->thread, NULL, display_run, displayp) != 0) {
        perror("play_thread create");
        return NULL;
    }

    return display;
}

void display_stop(display_t* display) {
    display->stop = true;

    if (pthread_join(display->thread, NULL) != 0)
        perror("display_thread join");

    free(display);
}