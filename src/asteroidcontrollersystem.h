#ifndef RUNNER_ASTEROID_CONTROLLER_SYSTEM_H
#define RUNNER_ASTEROID_CONTROLLER_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "movementcomponent.h"
#include "asteroidcontrollercomponent.h"

typedef struct asteroid_controller_system_t {
    AspectSystem super;
} AsteroidControllerSystem;

void asteroid_controller_system_init(AsteroidControllerSystem* self, EntityManager* entityManager);
void asteroid_controller_system_update(AsteroidControllerSystem* self);

void asteroid_controller_system_on_entity_removed(AspectSystem* system, Entity entity, const Message msg);

#endif