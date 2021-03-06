#ifndef ASTEROIDS_BG_MANAGER_COMPONENT
#define ASTEROIDS_BG_MANAGER_COMPONENT

#include "core.h"
#include "component.h"
#include "entity.h"
#include "transformcomponent.h"
#include "entitymanager.h"

typedef struct bg_manager_component_t {
    Component super;
    Prefab* tilePrefab;
    u32 rows;
    u32 columns;
    u32 tileWidth;
    u32 tileHeight;
    TransformComponent** transforms;
    u32 capacity;
    u32 count;
} BgManagerComponent;

BgManagerComponent* bg_manager_component_new(Entity entity);
COMPONENT_DEFINE(COMPONENT_BG_MANAGER);
bool bg_manager_component_add_entity(BgManagerComponent* self, EntityManager* entityManager, Entity entity);

#endif