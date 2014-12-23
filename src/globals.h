#ifndef RUNNER_GLOBALS_H
#define RUNNER_GLOBALS_H

#include "types.h"
#include "gametime.h"
#include "algebra.h"
#include "camera.h"
#include "texturemanager.h"
#include "config.h"
#include "tween.h"

#include <SDL2/SDL.h>

typedef struct entity_t Entity;

// All globals should exist within the globals struct
// Try to limit total number of globals

typedef struct globals {
    u32 randomSeed;

    struct screen_t {
        u32 width;
        u32 height;
    } screen;

    struct world_t {
        u32 width;
        u32 height;
    } world;

    Vec2 gravity;

    GameTime time;

    SDL_Window* window;
    SDL_Renderer* renderer;

    Camera camera;

    TextureManager textures;

    //TODO: some sort of named entity system
    Entity* player;

	TweenManager tweens;

    f32 scrollSpeed;
} Globals;

// Sets all the values in globals to reasonable defaults (0 most likely)
// Not intended to use as a configuration step.
void globals_init();
void globals_config(const char* configName);

void textures_init(const char* rootDir);
void textures_free();
bool textures_load(const char* filename);
bool textures_unload(const char* filename);
bool textures_unload_all();
SDL_Texture* textures_get(const char* name);

extern Globals globals;

#endif