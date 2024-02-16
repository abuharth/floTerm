#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#include "floTerm.h"

// TODO: add support for background colors, change names in the
// COLOR ENUM to not clog the namespace

// print a unicode character to the buffer at position (x, y)
// double wide characters not preferred as they offset the row
void buf_print_char(tbuf_t buf, int x, int y, wchar_t c, color_t color) {
    if (y > buf.height-1 || x > buf.width -1 || y < 0 || x < 0) {
        // termQuit(buf);
        printf("ERROR buf_print_char: coordinates out of range!\n");
        return;
    }
    buf.text[(y*buf.width) + x] = c;
    buf.color[(y*buf.width) + x] = color;
    // only overwrite the color if here was nothing set beneath
    if (buf.color[(y*buf.width) + x + 1] == BLANK) {
        buf.color[(y*buf.width) + x + 1] = DEFAULT;
    } 
}

// print a string of unicode characters to the buffer starting at position (x, y)
// double wide characters are not preferred as they offset the row
void buf_print_str(tbuf_t buf, int x, int y, wchar_t *s, color_t color) {
    if (y > buf.height-1 || x > buf.width -1 || y < 0 || x < 0) {
        // termQuit(buf);
        printf("ERROR buf_print_str: coordinates out of range!\n");
        return;
    }

    int j = 0;
    for (int i = 0; i < wcslen(s); i++) {
        if (s[i] == L'\n') {
            continue;
        }
        buf.text[(y*buf.width) + x + j] = s[i];
        buf.color[(y*buf.width) + x + j] = color;
        j += 1;
    }

    if (buf.color[(y*buf.width) + x + wcslen(s)] == BLANK) {
        buf.color[(y*buf.width) + x + wcslen(s)] = DEFAULT;
    } 
}

void clear_buf(tbuf_t buf, wchar_t c, color_t color) {
    for (int i = 0; i < buf.width*buf.height; i++) {
        buf.text[i] = c;
    }
    for (int i = 0; i < buf.width*buf.height; i++) {
        buf.color[i] = color;
    }
}

void apply_color(color_t color, color_t prev_color) {
    switch (color) {
        case DEFAULT:
            if (prev_color != DEFAULT) {
                printf("\e[39m");
            }
            break;
        case RED:
            if (prev_color != RED) {
                printf("\e[31m");
            }
            break;
        case GREEN:
            if (prev_color != GREEN) {
                printf("\e[32m");
            }
            break;
        case YELLOW:
            if (prev_color != YELLOW) {
                printf("\e[33m");
            }
            break;
        case BLUE:
            if (prev_color != BLUE) {
                printf("\e[34m");
            }
            break;
        case MAGENTA:
            if (prev_color != MAGENTA) {
                printf("\e[35m");
            }
            break;
        case CYAN:
            if (prev_color != CYAN) {
                printf("\e[36m");
            }
            break;
        default:
            break;
    }
}

void present_buf(tbuf_t buf) {
    for (int i = 0; i < buf.height; i++) {
        for (int j = 0; j < buf.width; j++) {
            // check color buffer for colors
            apply_color(buf.color[(i*buf.width) + j], buf.color[(i*buf.width) + j - 1]);

            printf("%lc", buf.text[(i*buf.width) + j]);
            if (j == buf.width-1) {
                printf("\n");
            }
        }
    }
    // cursor back to top and left
    printf("\e[%dA", buf.height);
    printf("\e[1000D");
}

void termInit(tbuf_t *buf, int width, int height) {
    buf->width = width;
    buf->height = height;

    // allocate memory for text and color buffer on the stack
    buf->text = (wchar_t*) malloc(buf->width*buf->height*sizeof(wchar_t));
    buf->color = (int*) malloc(buf->width*buf->height*sizeof(int));

    // set encoding
    setlocale(LC_ALL, "C.UTF-8");

    // hide cursor
    printf("\e[?25l");

    // fill buffer with whitespace
    clear_buf(*buf, L' ', BLANK);
}

void termQuit(tbuf_t buf) {
    // unhide cursor
    printf("\e[?25h");
    // reset color
    printf("\e[0m");
    // clear the game
    printf("\e[0J");

    // free resources from terminal buffer
    free(buf.text);
    free(buf.color);
}
