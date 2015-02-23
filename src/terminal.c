#include "terminal.h"

void terminal_set_color(TermColor color) {
    platform_terminal_set_color(color);
}

void terminal_set_bgcolor(TermColor color) {
    platform_terminal_set_bgcolor(color);
}

void terminal_reset_color() {
    platform_terminal_reset_color();
}