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
        clear_buf(buf, L'⋅', MAGENTA);
        buf_print_str(buf, buf.width/2 - 8, buf.height/2 - 2, L"┌─────────────┐", BLUE);
        buf_print_str(buf, buf.width/2 - 8, buf.height/2 - 1, L"│             │", BLUE);
        buf_print_str(buf, buf.width/2 - 7, buf.height/2 - 1, L"Hello, World!", YELLOW);
        buf_print_str(buf, buf.width/2 - 8, buf.height/2, L"└─────────────┘", BLUE);
        // --------- draw ----------

        present_buf(buf);
        usleep(100000);

        // catching interrupt signals like Ctrl-c
        signal(SIGINT, handle_signal);
        signal(SIGTSTP, handle_signal);

    }
    // clear screen, reset color mode, free buffer memory
    termQuit(buf);
    return 0;
}
