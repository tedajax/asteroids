#include "game.h"
#include "prefab.h"
#include "atlas.h"
#include "particles.h"
#include "scene.h"

#include <SDL2/SDL_image.h>

bool drawCollision = false;
bool playGame = true;

ParticleEmitter testParticle;
TransformComponent testParticleTransform;

void game_debug_keys(Game* self);

void game_init(Game* self) {  
    globals.game = self;

    component_system_init();
    tween_manager_init(&globals.tweens, 4192);

    textures_init("assets/textures");
    atlases_init();
    atlas_load("atlas1");

    prefab_system_init("assets/prefabs");
    scene_system_init("assets/scenes");

    self->activeScene = &self->playScene;

    game_scene_init(self->activeScene);

    scene_instantiate(scene_get("play.scene"), active_scene()->entityManager);

    for (u32 i = 0; i < 6; ++i) {
        entity_create_asteroid(self->activeScene->entityManager, (i / 2) + 3);
    }

    camera_init(&globals.camera, NULL);

    {
        debug_hud_init(&self->debugHud, "assets/fonts/terminus.ttf", 12);

        DebugHudWatch* fpsWatch = debug_hud_add_watch(&self->debugHud, "FPS", WATCH_TYPE_INT, &globals.time.fps);
        debug_hud_watch_set_warnings(fpsWatch, false, 120, 60);

        DebugHudWatch* frameTimeWatch = debug_hud_add_watch(&self->debugHud, "Frame MS", WATCH_TYPE_INT, &globals.time.per_frame_ms);
        debug_hud_watch_set_warnings(frameTimeWatch, true, 14, 16);

        debug_hud_add_watch(&self->debugHud, "Timescale", WATCH_TYPE_FLOAT, &globals.time.timescale);

        DebugHudWatch* entityWatch = debug_hud_add_watch(&self->debugHud, "Entities", WATCH_TYPE_INT, &self->activeScene->entityManager->entities.count);
        debug_hud_watch_set_warnings(entityWatch, true, 900, 1000);

        debug_hud_add_watch(&self->debugHud, "Level Position", WATCH_TYPE_FLOAT, &globals.levelPosition);

        DebugHudWatch* tweenWatch = debug_hud_add_watch(&self->debugHud, "Tweens", WATCH_TYPE_INT, &globals.tweens.count);
        debug_hud_watch_set_warnings(tweenWatch, true, 2500, 4000);
    }

    //////////////////////////////
    //emitter_init(&testParticle, CONFIG_GET(ParticleEmitterConfig)(config_get("particles.ini"), "particles", "bullet_explosion"));
    //////////////////////////////
}

void game_quit(Game* self) {
    scene_system_terminate();
    prefab_system_terminate();
    game_scene_quit(self->activeScene);
    atlases_terminate();
    textures_terminate();

    //////////////////////////////
    //emitter_free(&testParticle);
    //////////////////////////////
    
    tween_manager_terminate(&globals.tweens);
    debug_hud_free(&self->debugHud);
    component_system_terminate();
}

void game_start(Game* self) {
    game_scene_start(self->activeScene);
}

void game_update(Game* self) {
    if (input_key_down(SDL_SCANCODE_F5)) {
        playGame = true;
    }
    
    if (!playGame) {
        return;
    }

    globals.levelPosition += globals.scrollSpeed * globals.time.delta;

    game_scene_update(self->activeScene);

    camera_update(&globals.camera);

    //////////////////////////////
    //testParticleTransform.position.x = 500;
    //testParticleTransform.position.y = 300;
    //emitter_update(&testParticle, &testParticleTransform);
    //////////////////////////////
    
    debug_hud_update_surfaces(&self->debugHud, globals.renderer);
    tween_manager_update(&globals.tweens, globals.time.delta);

    game_debug_keys(self);
}

void game_debug_keys(Game* self) {
    if (input_key_down(SDL_SCANCODE_F1)) {
        debug_hud_dump(&self->debugHud, stdout);
    }

    if (input_key_down(SDL_SCANCODE_F2)) {
        profiler_dump(stdout, false);
    }

    if (input_key_down(SDL_SCANCODE_F3)) {
        drawCollision = !drawCollision;
    }

    if (input_key_down(SDL_SCANCODE_MINUS)) {
        globals.time.timescale -= 0.1f;
    }

    if (input_key_down(SDL_SCANCODE_EQUALS)) {
        globals.time.timescale += 0.1f;
    }

    if (input_key_down(SDL_SCANCODE_F12)) {
        DEBUG_BREAKPOINT();
    }
}

void game_render(Game* self) {
    /////////////////////////////////
    //emitter_render(&testParticle, &testParticleTransform);
    /////////////////////////////////

    game_scene_render(self->activeScene);

    if (drawCollision) {
        collision_system_render(&self->activeScene->collisionSystem);
    }

    debug_hud_render(&self->debugHud, globals.renderer, 5, 5);
}

void game_frame_end(Game* self) {
    game_scene_frame_end(self->activeScene);
}