#ifndef DISPLAY_H
#define DISPLAY_H

typedef struct display_t display_t;

display_t* display_start(int delay, int* wheels, int wheelcount);
void display_stop(display_t* wheel);
void display_join(display_t* wheel);

#endif