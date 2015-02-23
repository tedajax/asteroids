#include "../terminal.h"
#include <stdio.h>

int posix_get_color_int(TermColor color) {
    switch (color) {
        case TERM_COLOR_WHITE:      return 7;
        case TERM_COLOR_RED:        return 1;
        case TERM_COLOR_GREEN:      return 2;
        case TERM_COLOR_BLUE:       return 4;
        case TERM_COLOR_CYAN:       return 6;
        case TERM_COLOR_YELLOW:     return 3;
        case TERM_COLOR_MAGENTA:    return 5;
        case TERM_COLOR_BLACK:      return 0;
        default:                    return 7;
    }
}

void platform_terminal_set_color(TermColor color) {
    printf("\e[3%dm", posix_get_color_int(color));
}

void platform_terminal_set_bgcolor(TermColor color) {
    printf("\e[4%dm", posix_get_color_int(color));
}

void platform_terminal_reset_color() {
    printf("\e[0m");
}