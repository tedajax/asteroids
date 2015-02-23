#include "../terminal.h"
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

const WORD defaultAttributes = 7;

void platform_terminal_set_color(TermColor color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

    WORD attributes = consoleInfo.wAttributes & 0x78;
    if (color != TERM_COLOR_BLACK){
        attributes |= FOREGROUND_INTENSITY;
    }

    if (color == TERM_COLOR_WHITE || color == TERM_COLOR_RED || color == TERM_COLOR_YELLOW || color == TERM_COLOR_MAGENTA) {
        attributes |= FOREGROUND_RED;
    }

    if (color == TERM_COLOR_WHITE || color == TERM_COLOR_GREEN || color == TERM_COLOR_YELLOW || color == TERM_COLOR_CYAN) {
        attributes |= FOREGROUND_GREEN;
    }

    if (color == TERM_COLOR_WHITE || color == TERM_COLOR_BLUE || color == TERM_COLOR_CYAN || color == TERM_COLOR_MAGENTA) {
        attributes |= FOREGROUND_BLUE;
    }

    SetConsoleTextAttribute(hConsole, attributes);
}

void platform_terminal_set_bgcolor(TermColor color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

    WORD attributes = consoleInfo.wAttributes & 0xF;
    if (color != TERM_COLOR_BLACK) {
        attributes |= BACKGROUND_INTENSITY;
    }

    if (color == TERM_COLOR_WHITE || color == TERM_COLOR_RED || color == TERM_COLOR_YELLOW || color == TERM_COLOR_MAGENTA) {
        attributes |= BACKGROUND_RED;
    }

    if (color == TERM_COLOR_WHITE || color == TERM_COLOR_GREEN || color == TERM_COLOR_YELLOW || color == TERM_COLOR_CYAN) {
        attributes |= BACKGROUND_GREEN;
    }

    if (color == TERM_COLOR_WHITE || color == TERM_COLOR_BLUE || color == TERM_COLOR_CYAN || color == TERM_COLOR_MAGENTA) {
        attributes |= BACKGROUND_BLUE;
    }

    SetConsoleTextAttribute(hConsole, attributes);
}

void platform_terminal_reset_color() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, defaultAttributes);
}