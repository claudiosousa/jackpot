/**
 * Some helper functions for managing execution time.
 * @author Claudio Sousa, Gonzalez David
 */

#ifndef TIMER_H
#define TIMER_H

#include <time.h>

void timer_start(struct timespec *tm);
void timer_wait(struct timespec *start_time, int frequency);

#endif
