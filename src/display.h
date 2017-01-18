#ifndef DISPLAY_H
#define DISPLAY_H

typedef struct display_t display_t;

display_t* display_start(int* wheels, int wheelcount);
void display_stop(display_t* wheel);

#endif