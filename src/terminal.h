#ifndef RUNNER_TERMINAL_H
#define RUNNER_TERMINAL_H

typedef enum term_color_e {
    TERM_COLOR_WHITE,
    TERM_COLOR_RED,
    TERM_COLOR_GREEN,
    TERM_COLOR_BLUE,
    TERM_COLOR_CYAN,
    TERM_COLOR_MAGENTA,
    TERM_COLOR_YELLOW,
    TERM_COLOR_BLACK
} TermColor;

void platform_terminal_set_color(TermColor color);
void platform_terminal_set_bgcolor(TermColor color);
void platform_terminal_reset_color();

void terminal_set_color(TermColor color);
void terminal_set_bgcolor(TermColor color);
void terminal_reset_color();

#endif