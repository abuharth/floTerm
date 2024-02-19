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
    termInit(&buf, 30, 20);

    while (!quit) {
        // --------- draw ----------
        clear_buf(buf, L'•', P_WHITE_BLACK);
        buf_put_str(buf, buf.width/2 - 8, buf.height/2 - 2, L"┌─────────────┐", P_BLACK_WHITE);
        buf_put_str(buf, buf.width/2 - 8, buf.height/2 - 1, L"│             │", P_BLACK_WHITE);
        buf_put_str(buf, buf.width/2 - 7, buf.height/2 - 1, L"Hello, World!", P_BLACK_YELLOW);
        buf_put_str(buf, buf.width/2 - 8, buf.height/2, L"└─────────────┘", P_BLACK_WHITE);
        // --------- draw ----------

        // catching interrupt signals like Ctrl-c
        signal(SIGINT, handle_signal);
        signal(SIGTSTP, handle_signal);

        present_buf(buf);
        usleep(100000);
    }
    // clear screen, reset color mode, free buffer memory
    termQuit(&buf);
    return 0;
}
