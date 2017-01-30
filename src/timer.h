/**
 * Some helper functions for managing execution time.
 * @author Gonzalez David, Claudio Sousa
 */

#ifndef TIMER_H
#define TIMER_H

#include <time.h>

void timer_start(struct timespec *tm);
void timer_wait(struct timespec *start_time, int frequency);

#endif
