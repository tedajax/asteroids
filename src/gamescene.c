#include "gamescene.h"
#include "prefab.h"
#include "entityfactory.h"

void game_scene_init(GameScene* self) {
    self->entityManager = entity_manager_new();
    transform_system_init(&self->transformSystem, self->entityManager);
    health_system_init(&self->healthSystem, self->entityManager);
    sprite_system_init(&self->spriteSystem, self->entityManager);
    movement_system_init(&self->movementSystem, self->entityManager);
    gravity_system_init(&self->gravitySystem, self->entityManager);
    controller_system_init(&self->controllerSystem, self->entityManager);
    bullet_controller_system_init(&self->bulletControllerSystem, self->entityManager);
    bg_manager_system_init(&self->bgManagerSystem, self->entityManager);
    enemy_system_init(&self->enemySystem, self->entityManager);
    collision_system_init(&self->collisionSystem, self->entityManager);
    particle_system_init(&self->particleSystem, self->entityManager);
    lua_system_init(&self->luaSystem, self->entityManager);
    screen_wrap_system_init(&self->screenWrapSystem, self->entityManager);
    asteroid_controller_system_init(&self->asteroidControllerSystem, self->entityManager, config_get("game.ini"), "asteroid_config");
    ufo_controller_system_init(&self->ufoControllerSystem, self->entityManager);
    level_manager_system_init(&self->levelManagerSystem, self->entityManager);
}

void game_scene_load(GameScene* self, const char* sceneName) {

}

void game_scene_quit(GameScene* self) {
    entity_manager_free(self->entityManager);
}

void game_scene_start(GameScene* self) {
    bg_manager_system_start(&self->bgManagerSystem);
    collision_system_start(&self->collisionSystem);
    asteroid_controller_system_start(&self->asteroidControllerSystem);
    lua_system_start(&self->luaSystem);
    level_manager_system_start(&self->levelManagerSystem);
}

void game_scene_update(GameScene* self) {
    health_system_update(&self->healthSystem);
    sprite_system_update(&self->spriteSystem);
    controller_system_update(&self->controllerSystem);
    bullet_controller_system_update(&self->bulletControllerSystem);
    gravity_system_update(&self->gravitySystem);
    enemy_system_update(&self->enemySystem);
    bg_manager_system_update(&self->bgManagerSystem);
    lua_system_update(&self->luaSystem);
    asteroid_controller_system_update(&self->asteroidControllerSystem);
    ufo_controller_system_update(&self->ufoControllerSystem);
    particle_system_update(&self->particleSystem);
    movement_system_update(&self->movementSystem);
    screen_wrap_system_update(&self->screenWrapSystem);
    level_manager_system_update(&self->levelManagerSystem);
    collision_system_update(&self->collisionSystem);
}

void game_scene_render(GameScene* self) {
    sprite_system_render(&self->spriteSystem);
    particle_system_render(&self->particleSystem);
    lua_system_render(&self->luaSystem);
    //if (drawCollision) {
    //    collision_system_render(&self->collisionSystem);
    //}
}

void game_scene_frame_end(GameScene* self) {
    entities_update(self->entityManager);
}