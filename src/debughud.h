#ifndef ASTEROIDS_DEBUG_HUD_H
#define ASTEROIDS_DEBUG_HUD_H

#include "core.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef enum debug_hud_watch_type_e {
    WATCH_TYPE_INT,
    WATCH_TYPE_FLOAT,
    WATCH_TYPE_BOOL,
} DebugHudWatchType;

typedef union debug_hud_watch_value_u {
    int* i;
    float* f;
    bool* b;
} DebugHudWatchValue;

typedef enum debug_hud_watch_warning_type_e {
    DEBUG_HUD_WATCH_WARNING_NONE,
    DEBUG_HUD_WATCH_WARNING_ADDITIVE,
    DEBUG_HUD_WATCH_WARNING_SUBTRACTIVE,
} DebugHudWatchWarningType;

typedef struct debug_hud_watch_t {
    SDL_Texture* texture;
    char* displayName;
    DebugHudWatchType type;
    DebugHudWatchValue value;
    DebugHudWatchWarningType warningType;
    int warningThreshold;
    int errorThreshold;
} DebugHudWatch;

void debug_hud_watch_build_string(DebugHudWatch* self, char* dest, u32 max);

typedef struct debug_hud_t {
    TTF_Font* debugFont;
    int ptSize;
    DebugHudWatch* watches;
    u32 count;
    u32 capacity;
} DebugHud;

void debug_hud_init(DebugHud* self, const char* fontName, int ptSize);
void debug_hud_free(DebugHud* self);
DebugHudWatch* debug_hud_add_watch(DebugHud* self, char* name, DebugHudWatchType type, void* value);
void debug_hud_watch_set_warnings(DebugHudWatch* self, bool additiveWarnings, int warning, int error);
void debug_hud_update_surfaces(DebugHud* self, SDL_Renderer* renderer);
void debug_hud_render(DebugHud* self, SDL_Renderer* renderer, int x, int y);

void debug_hud_dump(DebugHud* self, FILE* f);

#endif