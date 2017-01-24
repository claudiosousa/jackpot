/**
 * Main program.
 * @author Claudio Sousa, Gonzalez David
 */

#include <stdio.h>
#include "controller.h"


int main() {
    setbuf(stdout, 0);
    return controller_start();
}
