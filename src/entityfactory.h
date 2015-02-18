#ifndef RUNNER_ENTITY_FACTORY_H
#define RUNNER_ENTITY_FACTORY_H

#include "entity.h"
#include "entitymanager.h"
#include "components.h"
#include "systems.h"

#include <SDL2/SDL.h>

typedef struct bullet_source_config_t BulletSourceConfig;

Entity entity_create_player(EntityManager* entityManager, Vec2 position, Atlas* atlas, char* spriteName);
Entity entity_create_bullet(EntityManager* entityManager, BulletConfig* bulletConfig, ColliderConfig* colliderConfig, Vec2 position, Atlas* atlas, char* spriteName);
Entity entity_create_bg_tile(EntityManager* entityManager, Atlas* atlas, char* spriteName);
Entity entity_create_bg_manager(EntityManager* entityManager, u32 twidth, u32 theight);
Entity entity_create_basic_enemy(EntityManager* entityManager, Vec2 position);
Entity entity_create_asteroid(EntityManager* entityManager, Vec2 position, i32 size);

#endif