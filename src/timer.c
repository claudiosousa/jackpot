/**
 * Some helper functions for managing execution time.
 * @author Gonzalez David, Claudio Sousa
 */

#include <stdio.h>
#include <unistd.h>
#include "timer.h"

/**
 * Starts a time wait
 * @param tm Structure to hold the starting time
 */
void timer_start(struct timespec *tm) {
    if (clock_gettime(CLOCK_MONOTONIC, tm) != 0)
        perror("Time start failed");
}

/**
 * Make the calling thread wait the time remaining
 * @param start_time Starting time
 * @param ms Delay in ms
 */
void timer_wait(struct timespec *start_time, int ms) {
    int wait_time = ms * 1000;  //µs
    struct timespec end_time;

    if (clock_gettime(CLOCK_MONOTONIC, &end_time) == 0) {
        wait_time -= (end_time.tv_sec - start_time->tv_sec) * 1000000;
        wait_time -= (end_time.tv_nsec - start_time->tv_nsec) / 1000;
        if (wait_time > 0) {
            usleep(wait_time);
        }
    } else {
        perror("Time stop failed");
    }

    timer_start(start_time);
}
