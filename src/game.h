#ifndef ASTEROIDS_GAME_H
#define ASTEROIDS_GAME_H

#include "core.h"
#include "entitymanager.h"
#include "entityfactory.h"
#include "components.h"
#include "systems.h"
#include "debughud.h"
#include "gamescene.h"

#define GAME_MAX_SCENES 8

typedef struct game_t {
    GameScene* activeScene;
    GameScene sceneStack[GAME_MAX_SCENES];
    u32 sceneHead;
    DebugHud debugHud;
    InputManager globalInput;
} Game;

void game_init(Game* self);
void game_quit(Game* self);
void game_start(Game* self);
void game_update(Game* self);
void game_render(Game* self);
void game_frame_end(Game* self);

GameScene* game_push_scene(Game* self);
GameScene* game_scene_pop(Game* self);

void game_load_scene(Game* self, const char* sceneName);

#endif