#include "gamescene.h"
#include "prefab.h"
#include "entityfactory.h"

void game_scene_init(GameScene* self) {
    self->entityManager = entity_manager_new();
    
    self->timerManager = CALLOC(1, TimerManager);
    timer_manager_init(self->timerManager, self->entityManager);

    input_initialize(&self->input);

    transform_system_init(&self->transformSystem, self);
    health_system_init(&self->healthSystem, self);
    sprite_system_init(&self->spriteSystem, self);
    movement_system_init(&self->movementSystem, self);
    gravity_system_init(&self->gravitySystem, self);
    controller_system_init(&self->controllerSystem, self);
    bullet_controller_system_init(&self->bulletControllerSystem, self);
    bg_manager_system_init(&self->bgManagerSystem, self);
    enemy_system_init(&self->enemySystem, self);
    collision_system_init(&self->collisionSystem, self);
    particle_system_init(&self->particleSystem, self);
    screen_wrap_system_init(&self->screenWrapSystem, self);
    asteroid_controller_system_init(&self->asteroidControllerSystem, self, config_get("game.ini"), "asteroid_config");
    ufo_controller_system_init(&self->ufoControllerSystem, self);
    level_manager_system_init(&self->levelManagerSystem, self);
}

void game_scene_load(GameScene* self, const char* sceneName) {

}

void game_scene_quit(GameScene* self) {
    timer_manager_terminate(self->timerManager);
    free(self->timerManager);

    entity_manager_free(self->entityManager);
}

void game_scene_start(GameScene* self) {
    bg_manager_system_start(&self->bgManagerSystem);
    collision_system_start(&self->collisionSystem);
    asteroid_controller_system_start(&self->asteroidControllerSystem);
    level_manager_system_start(&self->levelManagerSystem);
}

void game_scene_update(GameScene* self) {
    timer_manager_tick(self->timerManager, globals.time.delta);

    health_system_update(&self->healthSystem);
    sprite_system_update(&self->spriteSystem);
    controller_system_update(&self->controllerSystem);
    bullet_controller_system_update(&self->bulletControllerSystem);
    gravity_system_update(&self->gravitySystem);
    enemy_system_update(&self->enemySystem);
    bg_manager_system_update(&self->bgManagerSystem);
    asteroid_controller_system_update(&self->asteroidControllerSystem);
    ufo_controller_system_update(&self->ufoControllerSystem);
    particle_system_update(&self->particleSystem);
    movement_system_update(&self->movementSystem);
    screen_wrap_system_update(&self->screenWrapSystem);
    level_manager_system_update(&self->levelManagerSystem);
    collision_system_update(&self->collisionSystem);

    input_update(&self->input);
}

void game_scene_render(GameScene* self) {
    sprite_system_render(&self->spriteSystem);
    particle_system_render(&self->particleSystem);
    //if (drawCollision) {
    //    collision_system_render(&self->collisionSystem);
    //}
}

void game_scene_frame_end(GameScene* self) {
    entities_update(self->entityManager);
}