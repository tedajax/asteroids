#ifndef RUNNER_COLLISION_H
#define RUNNER_COLLISION_H

#include "aspectsystem.h"
#include "component.h"
#include "collidercomponent.h"
#include "collider.h"

#define COLLISION_MAX_COLLIDERS 1024

typedef struct collision_system_t {
    AspectSystem super;
    i32 currentId;
    size_t count;
    Collider* colliders[COLLISION_MAX_COLLIDERS];
} CollisionSystem;

void collision_system_init(CollisionSystem* self, EntityManager* entityManager);
i32 collision_system_gen_id(CollisionSystem* self);
void collision_system_register_collider(CollisionSystem* self, Collider* collider);
void collision_system_unregister_collider(CollisionSystem* self, i32 id);
void collision_system_update(CollisionSystem* self, EntityList* entities);

#endif