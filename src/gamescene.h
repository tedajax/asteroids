#ifndef ASTEROIDS_GAME_SCENE_H
#define ASTEROIDS_GAME_SCENE_H

#include "core.h"
#include "entitymanager.h"
#include "components.h"
#include "systems.h"
#include "timer.h"

typedef enum game_scene_state_e {
    GAME_SCENE_STATE_NONE       = 0x0,
    GAME_SCENE_STATE_UPDATE     = 0x1,
    GAME_SCENE_STATE_DRAW       = 0x2,
} GameSceneState;

typedef struct game_scene_t {
    GameSceneState state;

    TimerManager* timerManager;

    EntityManager* entityManager;
    InputManager input;

    TransformSystem transformSystem;
    MovementSystem movementSystem;
    ControllerSystem controllerSystem;
    BulletControllerSystem bulletControllerSystem;
    GravitySystem gravitySystem;
    HealthSystem healthSystem;
    SpriteSystem spriteSystem;
    BgManagerSystem bgManagerSystem;
    EnemySystem enemySystem;
    CollisionSystem collisionSystem;
    ParticleSystem particleSystem;
    ScreenWrapSystem screenWrapSystem;
    AsteroidControllerSystem asteroidControllerSystem;
    UfoControllerSystem ufoControllerSystem;
    LevelManagerSystem levelManagerSystem;
} GameScene;

void game_scene_init(GameScene* self);
void game_scene_load(GameScene* self, const char* sceneName);
void game_scene_quit(GameScene* self);
void game_scene_start(GameScene* self);
void game_scene_update(GameScene* self);
void game_scene_render(GameScene* self);
void game_scene_frame_end(GameScene* self);

#endif