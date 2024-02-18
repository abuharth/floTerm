#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#include "floTerm.h"

// color pair default definitions
colorpair_t P_BLACK             = { CBLACK, CDEFAULT };  
colorpair_t P_RED               = { CRED, CDEFAULT };
colorpair_t P_GREEN             = { CGREEN, CDEFAULT };
colorpair_t P_YELLOW            = { CYELLOW, CDEFAULT };
colorpair_t P_BLUE              = { CBLUE, CDEFAULT };
colorpair_t P_MAGENTA           = { CMAGENTA, CDEFAULT };
colorpair_t P_CYAN              = { CCYAN, CDEFAULT };
colorpair_t P_WHITE             = { CWHITE, CDEFAULT };
colorpair_t P_DEFAULT           = { CDEFAULT, CDEFAULT };

colorpair_t P_BLACK_WHITE       = { CBLACK, CWHITE };
colorpair_t P_BLACK_YELLOW      = { CBLACK, CYELLOW };
colorpair_t P_WHITE_BLACK       = { CWHITE, CBLACK };
colorpair_t P_YELLOW_BLACK      = { CYELLOW, CBLACK };

// print a unicode character to the buffer at position (x, y)
// double wide characters not preferred as they offset the row
int buf_print_char(tbuf_t buf, int x, int y, wchar_t c, colorpair_t color) {
    if (y > buf.height-1 || x > buf.width -1 || y < 0 || x < 0) {
        return -1;
    }
    buf.text[(y*buf.width) + x] = c;
    buf.fg_color[(y*buf.width) + x] = color.fg;
    buf.bg_color[(y*buf.width) + x] = color.bg;
    // only overwrite the color if here was nothing set beneath
    if (buf.fg_color[(y*buf.width) + x + 1] == CBLANK) {
        buf.fg_color[(y*buf.width) + x + 1] = CDEFAULT;
    }
    if (buf.bg_color[(y*buf.width) + x + 1] == CBLANK) {
        buf.bg_color[(y*buf.width) + x + 1] = CDEFAULT;
    }
    return 0;
}

// print a string of unicode characters to the buffer starting at position (x, y)
// double wide characters are not preferred as they offset the row
int buf_print_str(tbuf_t buf, int x, int y, wchar_t *s, colorpair_t color) {
    if (y > buf.height-1 || x > buf.width -1 || y < 0 || x < 0) {
        return -1;
    }

    int j = 0;
    for (int i = 0; i < wcslen(s); i++) {
        if (s[i] == L'\n') {
            continue;
        }
        buf.text[(y*buf.width) + x + j] = s[i];
        buf.fg_color[(y*buf.width) + x + j] = color.fg;
        buf.bg_color[(y*buf.width) + x + j] = color.bg;
        j += 1;
    }

    if (buf.fg_color[(y*buf.width) + x + wcslen(s)] == CBLANK) {
        buf.fg_color[(y*buf.width) + x + wcslen(s)] = CDEFAULT;
    } 
    if (buf.bg_color[(y*buf.width) + x + wcslen(s)] == CBLANK) {
        buf.bg_color[(y*buf.width) + x + wcslen(s)] = CDEFAULT;
    } 
    return 0;
}

void clear_buf(tbuf_t buf, wchar_t c, colorpair_t color) {
    for (int i = 0; i < buf.width*buf.height; i++) {
        buf.text[i] = c;
    }
    for (int i = 0; i < buf.width*buf.height; i++) {
        buf.fg_color[i] = color.fg;
        buf.bg_color[i] = color.bg;
    }
}

void apply_color(int fg_flag, color_t color, color_t prev_color) {
    switch (color) {
        case CBLACK:
            if (prev_color != CBLACK) {
                if (fg_flag == 1) printf("\e[30m");
                else printf("\e[40m");
            }
            break;
        case CRED:
            if (prev_color != CRED) {
                if (fg_flag == 1) printf("\e[31m");
                else printf("\e[41m");
            }
            break;
        case CGREEN:
            if (prev_color != CGREEN) {
                if (fg_flag == 1) printf("\e[32m");
                else printf("\e[42m");
            }
            break;
        case CYELLOW:
            if (prev_color != CYELLOW) {
                if (fg_flag == 1) printf("\e[33m");
                else printf("\e[43m");
            }
            break;
        case CBLUE:
            if (prev_color != CBLUE) {
                if (fg_flag == 1) printf("\e[34m");
                else printf("\e[44m");
            }
            break;
        case CMAGENTA:
            if (prev_color != CMAGENTA) {
                if (fg_flag == 1) printf("\e[35m");
                else printf("\e[45m");
            }
            break;
        case CCYAN:
            if (prev_color != CCYAN) {
                if (fg_flag == 1) printf("\e[36m");
                else printf("\e[46m");
            }
            break;
        case CWHITE:
            if (prev_color != CWHITE) {
                if (fg_flag == 1) printf("\e[37m");
                else printf("\e[47m");
            }
            break;
        case CDEFAULT:
            if (prev_color != CDEFAULT) {
                if (fg_flag == 1) printf("\e[39m");
                else printf("\e[49m");
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
            apply_color(1, buf.fg_color[(i*buf.width) + j], buf.fg_color[(i*buf.width) + j - 1]);
            apply_color(0, buf.bg_color[(i*buf.width) + j], buf.bg_color[(i*buf.width) + j - 1]);

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
    buf->fg_color = (int*) malloc(buf->width*buf->height*sizeof(int));
    buf->bg_color = (int*) malloc(buf->width*buf->height*sizeof(int));


    // set encoding
    setlocale(LC_ALL, "C.UTF-8");

    // hide cursor
    printf("\e[?25l");

    // print enough blankspace to fit the  buffer
    // then return to the top
    for (int i = 0; i < buf->height; i++) {
        printf("\n");
    }
    printf("\e[%dA", buf->height);

    // fill buffer with whitespace
    colorpair_t blank = {CBLANK, CBLANK};
    clear_buf(*buf, L' ', blank);
}

void termQuit(tbuf_t *buf) {
    // unhide cursor
    printf("\e[?25h");
    // reset color
    printf("\e[0m");
    // clear the game
    printf("\e[0J");

    // free resources from terminal buffer
    free(buf->text);
    free(buf->fg_color);
    free(buf->bg_color);
}
