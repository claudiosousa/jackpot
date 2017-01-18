/**
 * Main program.
 * @author Claudio Sousa, Gonzalez David
 */

#include <stdio.h>
#include "controller.h"

/**
 * Start game controller and waits for SIGQUIT signal
 * @return Program status
 */
int main() {
    setbuf(stdout, 0);
    return controller_start();
}
