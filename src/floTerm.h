// floTerm:
// A simple terminal drawing library written in C with no external dependencies

#ifndef FLOTERM_H
#define FLOTERM_H

#include <wchar.h>

typedef struct {
    wchar_t *text;
    int *fg_color;
    int *bg_color;
    int width;
    int height;
} tbuf_t;

typedef enum {
    CBLANK,
    CBLACK,
    CRED,
    CGREEN,
    CYELLOW,
    CBLUE,
    CMAGENTA,
    CCYAN,
    CWHITE,
    CDEFAULT,
} color_t;

typedef struct {
    color_t fg;
    color_t bg;
} colorpair_t;

// predefined color pairs for convenience
// user can define custom color pairs
// by using the colorpair_t struct
extern colorpair_t P_BLACK_FG;
extern colorpair_t P_RED_FG;
extern colorpair_t P_GREEN_FG;
extern colorpair_t P_YELLOW_FG;
extern colorpair_t P_BLUE_FG;
extern colorpair_t P_MAGENTA_FG;
extern colorpair_t P_CYAN_FG;
extern colorpair_t P_WHITE_FG;
extern colorpair_t P_DEFAULT_FG;

extern colorpair_t P_BLACK_BG;
extern colorpair_t P_RED_BG;
extern colorpair_t P_GREEN_BG;
extern colorpair_t P_YELLOW_BG;
extern colorpair_t P_BLUE_BG;
extern colorpair_t P_MAGENTA_BG;
extern colorpair_t P_CYAN_BG;
extern colorpair_t P_WHITE_BG;
extern colorpair_t P_DEFAULT_BG;

extern colorpair_t P_BLACK_WHITE;
extern colorpair_t P_BLACK_YELLOW;
extern colorpair_t P_WHITE_BLACK;
extern colorpair_t P_YELLOW_BLACK;

// floTerm functions
int buf_put_char(tbuf_t buf, int x, int y, wchar_t c, colorpair_t color);
int buf_put_str(tbuf_t buf, int x, int y, wchar_t *s, colorpair_t color);
int buf_put_rect(tbuf_t buf, int x, int y, int w, int h, colorpair_t color);
int buf_put_filled_rect(tbuf_t buf, int x, int y, int w, int h, wchar_t c, colorpair_t color);
void clear_buf(tbuf_t buf, wchar_t c, colorpair_t color);
void present_buf(tbuf_t buf);

void termInit(tbuf_t *buf, int width, int height);
void termQuit(tbuf_t *buf);

#endif
