// Example of using floTerm with input handling using termios.h, stdio.h and stdlib.h
// Use WASD to move the sprite around, q to quit

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include "floTerm.h"

struct termios oldt, newt;
tbuf_t buf;

int quit = 0;
int posx = 5;
int posy = 5;

// signal handling
void handle_signal(int sig) {
    quit = 1;
}

// rudamentary input handling
void process_input() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    if (FD_ISSET(STDIN_FILENO, &fds)) {
        int ch = getchar();
        if (ch == 'q' || ch == 27) {
            quit = 1;
        }
        if (ch == 'w') {
            posy -= 1;
        }
        if (ch == 's') {
            posy += 1;
        }
        if (ch == 'a') {
            posx -= 1;
        }
        if (ch == 'd') {
            posx += 1;
        }
    }
}

int main() {
    // termios: switch to canonical mode, disable echo
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // initialize terminal buffer with width and height 30x20
    terminit(&buf, 40, 20);
    while (!quit) {
        process_input();

        // --------- draw ----------
        bufclear(buf, L'⋅', P_BLACK_WHITE);
        addrect(buf, buf.width/2 - 8, buf.height/2 - 2, 15, 3, P_CYAN_BG);
        addstr(buf, buf.width/2 - 7, buf.height/2 - 1, L"Hello, World!", P_MAGENTA_BG);
        addchar(buf, posx, posy, L'☻', P_YELLOW_BLACK);
        // --------- draw ----------

        // catching interrupt signals like Ctrl-c
        signal(SIGINT, handle_signal);
        signal(SIGTSTP, handle_signal);

        bufpresent(buf);
        usleep(100000);

    }

    // set terminal settings back to normal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // clear screen, reset color mode, free buffer memory
    termquit(&buf);
    return 0;
}
