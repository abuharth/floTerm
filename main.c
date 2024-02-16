#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <wchar.h>
#include <locale.h>

#define WIDTH 30
#define HEIGHT 15

typedef struct {
    wchar_t *text;
    int *color;
} tbuf_t;

typedef enum {
    BLANK,
    DEFAULT,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
} color_t;

// global variables
int quit = 0;
struct termios oldt, newt;
// terminal buffer
tbuf_t buf;

int posx, posy;

// functions
void quit_term(void);

// print a character to the screen at position (x, y)
void buf_print_char(tbuf_t buf, int x, int y, wchar_t c, color_t color) {
    if (y > HEIGHT-1 || x > WIDTH -1 || y < 0 || x < 0) {
        quit_term();
        printf("ERROR buf_print_char: coordinates out of range!\n");
        exit(1);
    }
    buf.text[(y*WIDTH) + x] = c;
    buf.color[(y*WIDTH) + x] = color;
    buf.color[(y*WIDTH) + x + 1] = DEFAULT;
}

// print a string to the screen starting at position (x, y)
void buf_print_str(tbuf_t buf, int x, int y, wchar_t* s, color_t color) {
    if (y > HEIGHT-1 || x > WIDTH -1 || y < 0 || x < 0) {
        quit_term();
        printf("ERROR buf_print_str: coordinates out of range!\n");
        exit(1);
    }

    int j = 0;
    for (int i = 0; i < wcslen(s); i++) {
        if (s[i] == L'\n') {
            continue;
        }
        buf.text[(y*WIDTH) + x + j] = s[i];
        j += 1;
    }
    buf.color[(y*WIDTH) + x] = color;
    buf.color[(y*WIDTH) + x + wcslen(s)] = DEFAULT;
}

void clear_buf(tbuf_t buf, wchar_t c) {
    for (int i = 0; i < WIDTH*HEIGHT; i++) {
        buf.text[i] = c;
    }
    for (int i = 0; i < WIDTH*HEIGHT; i++) {
        buf.color[i] = 0;
    }
}

void apply_color(color_t color) {
    switch (color) {
        case DEFAULT:
            printf("\e[39m");
            break;
        case RED:
            printf("\e[31m");
            break;
        case GREEN:
            printf("\e[32m");
            break;
        case YELLOW:
            printf("\e[33m");
            break;
        case BLUE:
            printf("\e[34m");
            break;
        case MAGENTA:
            printf("\e[35m");
            break;
        case CYAN:
            printf("\e[36m");
            break;
        default:
            break;
    }
}

void print_buf(tbuf_t buf) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            // check color buffer for colors
            apply_color(buf.color[(i*WIDTH) + j]);

            printf("%lc", buf.text[(i*WIDTH) + j]);
            if (j == WIDTH-1) {
                printf("\n");
            }
        }
    }
}

void init() {
    // termios: switch to canonical mode, disable echo
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // allocate memory for text and color buffer on the stack
    buf.text = (wchar_t*) malloc(WIDTH*HEIGHT*sizeof(wchar_t));
    buf.color = (int*) malloc(WIDTH*HEIGHT*sizeof(int));

    // set encoding
    setlocale(LC_ALL, "C.UTF-8");
    // hide cursor
    printf("\e[?25l");

    // fill buffer with whitespace
    clear_buf(buf, L' ');
}

// TODO: there is probably a better way to take input
// want to remove the buffer, also take arrow keys
// as input

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

void quit_term() {
    printf("\e[?25h");
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\e[%dB", HEIGHT);

}


int main() {
    init();
    while (!quit) {
        process_input();

        // --------- draw ----------
        clear_buf(buf, L'⋅');
        buf_print_char(buf, posx, posy, L'☻', GREEN);
        buf_print_str(buf, posx, posy + 2, L"ahmed", RED);
        print_buf(buf);
        printf("\e[%dA", HEIGHT);

        usleep(100000);
    }

    quit_term();

    return 0;
}
