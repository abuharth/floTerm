// floTerm:
// A simple terminal drawing library written in C with no external dependencies

#ifndef FLOTERM_H
#define FLOTERM_H

#include <wchar.h>

typedef struct {
    wchar_t *text;
    int *color;
    int width;
    int height;
} tbuf_t;

typedef enum {
    BLANK,
    DEFAULT,
    WHITE,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
} color_t;

int buf_print_char(tbuf_t buf, int x, int y, wchar_t c, color_t color);
int buf_print_str(tbuf_t buf, int x, int y, wchar_t *s, color_t color);
void clear_buf(tbuf_t buf, wchar_t c, color_t color);
void present_buf(tbuf_t buf);

void termInit(tbuf_t *buf, int width, int height);
void termQuit(tbuf_t buf);

#endif
