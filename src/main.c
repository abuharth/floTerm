// Very simple example of using floTerm.h. All you need is to include the floTerm header,
// unistd.h for the usleep() function, and signal.h to handle exit signals

#include <unistd.h>
#include <signal.h>
#include "floTerm.h"

tbuf_t buf;
int quit = 0;

// signal handling
void handle_signal(int sig) {
    quit = 1;
}

int main() {
    // initialize terminal buffer with width and height 30x20
    terminit(&buf, 40, 20);

    while (!quit) {
        // --------- draw ----------
        bufclear(buf, L'•', P_BLUE_BG);
        addrect(buf, 0, 0, buf.width, buf.height, P_DEFAULT_FG);
        addstr(buf, buf.width/2 - 7, buf.height/2 - 1, L"Hello, World!", P_BLACK_YELLOW);
        addrect(buf, buf.width/2 - 8, buf.height/2 - 2, 15, 3, P_GREEN_BG);
        // --------- draw ----------

        // catching interrupt signals like Ctrl-c
        signal(SIGINT, handle_signal);
        signal(SIGTSTP, handle_signal);

        bufpresent(buf);
        usleep(100000);
    }
    // clear screen, reset color mode, free buffer memory
    termquit(&buf);
    return 0;
}
