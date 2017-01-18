#ifndef WHEEL_H
#define WHEEL_H

typedef struct wheel_t wheel_t;

wheel_t * wheel_start(int delay, int* wheel);
void wheel_stop(wheel_t* wheel);

#endif