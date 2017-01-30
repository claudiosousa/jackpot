/**
 * Game structures shared accross modules
 * @author David Gonzalez, Claudio Sousa
 */
#include <stdio.h>
#include "controller.h"

/** App entry point */
int main() {
    setbuf(stdout, 0);
    return controller_start();
}
