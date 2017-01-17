/**
 * Main program.
 * @author Claudio Sousa, Gonzalez David
 */

#include <stdlib.h>
#include "controller.h"

/**
 * Start game controller and waits for SIGQUIT signal
 * @return Program status
 */
int main() {
    return controller_start();
}
