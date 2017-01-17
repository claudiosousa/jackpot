#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

volatile sig_atomic_t etst;

static void controller_coin_inserted(int signum) {
    (void)signum;
    char msg[] = "SIGINT received! Aborted.\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    _exit(EXIT_SUCCESS);
}

void controller_start() {
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = controller_coin_inserted;

    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    int count = 0;
    while (1) {
        printf("Line %d\n", ++count);
        sleep(1);
    }

    // SIGQUIT
    // SIGTSTP
}
/*


BLOCK
sigset_t mask;
sigfillset(&mask);
pthread_sigmask(SIG_SETMASK, &mask, NULL);

SET HANDLE
struct sigaction act;
memset(&act, 0, sizeof(act));
act.sa_handler = handler;
sigaction(SIGINT, &act, NULL);

UINLOCK
sigdelset(&mask, SIGINT);
pthread_sigmask(SIG_SETMASK, &mask, NULL);

//wait signal
sigwait



//WAIT
sigset_t mask, maskold;
sigfillset(&mask);
pthread_sigmask(SIG_SETMASK, &mask, &maskold);

sigwait(&mask, &sig);

//SIGALRM
unsigned int alarm(unsigned int seconds);
*/