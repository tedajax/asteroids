#ifndef ASTEROIDS_LEVEL_MANAGER_SYSTEM_H
#define ASTEROIDS_LEVEL_MANAGER_SYSTEM_H

#include "aspectsystem.h"
#include "levelmanagercomponent.h"

typedef struct level_manager_system_t {
    AspectSystem super;
} LevelManagerSystem;

void level_manager_system_init(LevelManagerSystem* self, GameScene* scene);
void level_manager_system_start(LevelManagerSystem* self);
void level_manager_system_update(LevelManagerSystem* self);

void level_manager_on_entity_added(AspectSystem* system, Entity entity, const Message msg);
void level_manager_on_entity_removed(AspectSystem* system, Entity entity, const Message msg);
void level_manager_on_spawn_player(AspectSystem* system, Entity entity, const Message msg);

#endif