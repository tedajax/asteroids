#ifndef RUNNER_ASTEROID_CONTROLLER_SYSTEM_H
#define RUNNER_ASTEROID_CONTROLLER_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "movementcomponent.h"
#include "asteroidcontrollercomponent.h"
#include "config.h"

typedef struct asteroid_controller_system_t {
    AspectSystem super;
    i32 maxSize;
    f32 normalSpeedMultiplier;
    f32 maxSpeedMultiplier;
    f32 friction;
} AsteroidControllerSystem;

void asteroid_controller_system_init(AsteroidControllerSystem* self, EntityManager* entityManager, Config* config, const char* table);
void asteroid_controller_system_start(AsteroidControllerSystem* self);
void asteroid_controller_system_update(AsteroidControllerSystem* self);

void asteroid_controller_system_on_entity_removed(AspectSystem* system, Entity entity, const Message msg);
void asteroid_controller_system_on_collision_enter(AspectSystem* system, Entity entity, const Message msg);

#endif