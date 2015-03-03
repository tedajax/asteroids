#include "log.h"
#include "terminal.h"
#include "snprintf.h"
#include "debug.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "directory.h"
#include "globals.h"

FILE* logFile = NULL;

void log_init() {
    // Check that a logs directory exists and if not make one.
    struct stat logDirStat = { 0 };
    if (stat("logs", &logDirStat) == -1) {
        IF_DEBUG(bool result = )directory_create("logs");
        ASSERT(result, "Failed to create logs directory.");
    }

    char filename[128];
    snprintf(filename, 128, "logs/log_%ld.txt", time(NULL));
    logFile = fopen(filename, "w");
    ASSERT(logFile, "Failed to open logging file.");
}

void log_terminate() {
    fclose(logFile);
}

void log_log(LogLevel level, const char* context, const char* msg) {
    _set_console_color_level(level);

    char buffer[1024];
    snprintf(buffer, 1024, "[%s] %s\n", context, msg);

    fprintf(stdout, buffer);
    fprintf(logFile, "%lld %s", game_time_nano_to_milli(globals.time.since_start_ns), buffer);

    _reset_console_color();

    if (level == LOG_LEVEL_PANIC) {
        DEBUG_BREAKPOINT();
    }
}

void log_log_format(LogLevel level, const char* context, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _log_formatv(level, context, format, args);
    va_end(args);
}

void log_info(const char* context, const char* msg) {
    log_log(LOG_LEVEL_INFO, context, msg);
}

void log_info_format(const char* context, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _log_formatv(LOG_LEVEL_INFO, context, format, args);
    va_end(args);
}

void log_warning(const char* context, const char* msg) {
    log_log(LOG_LEVEL_WARNING, context, msg);
}

void log_warning_format(const char* context, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _log_formatv(LOG_LEVEL_WARNING, context, format, args);
    va_end(args);
}

void log_error(const char* context, const char* msg) {
    log_log(LOG_LEVEL_ERROR, context, msg);
}

void log_error_format(const char* context, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _log_formatv(LOG_LEVEL_ERROR, context, format, args);
    va_end(args);
}

void log_panic(const char* context, const char* msg) {
    log_log(LOG_LEVEL_PANIC, context, msg);
}

void log_panic_format(const char* context, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _log_formatv(LOG_LEVEL_PANIC, context, format, args);
    va_end(args);
}

// Private
void _log_formatv(LogLevel level, const char* context, const char* format, va_list args) {
    const u32 MAX_LENGTH = 256;
    char msg[256];

    vsnprintf(&msg[0], MAX_LENGTH, format, args);

    log_log(level, context, msg);
}

void _set_console_color_level(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_INFO:
            terminal_set_color(TERM_COLOR_WHITE);
            break;

        case LOG_LEVEL_WARNING:
            terminal_set_color(TERM_COLOR_YELLOW);
            break;

        case LOG_LEVEL_ERROR:
            terminal_set_color(TERM_COLOR_RED);
            break;

        case LOG_LEVEL_PANIC:
            terminal_set_color(TERM_COLOR_BLACK);
            terminal_set_bgcolor(TERM_COLOR_RED);
            break;

        default: break;
    }
}

void _reset_console_color() {
    terminal_reset_color();
}