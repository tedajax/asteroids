#ifndef ASTEROIDS_ENTITY_FACTORY_H
#define ASTEROIDS_ENTITY_FACTORY_H

#include "entity.h"
#include "entitymanager.h"
#include "components.h"
#include "systems.h"

#include <SDL2/SDL.h>

Entity entity_create_player(EntityManager* entityManager, Vec2 position);
Entity entity_create_bullet(EntityManager* entityManager, Vec2 position, f32 baseAngle);
Entity entity_create_bg_manager(EntityManager* entityManager);
Entity entity_create_basic_enemy(EntityManager* entityManager, Vec2 position);
Entity entity_create_asteroid(EntityManager* entityManager, char* prefabName);

#endif