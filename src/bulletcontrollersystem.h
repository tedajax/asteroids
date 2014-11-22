#ifndef RUNNER_BULLET_CONTROLLER_SYSTEM_H
#define RUNNER_BULLET_CONTROLLER_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "movementcomponent.h"
#include "bulletcontrollercomponent.h"

typedef struct bullet_controller_system_t {
    AspectSystem super;
} BulletControllerSystem;

BulletControllerSystem* bullet_controller_system_new(EntityManager* entityManager);
void bullet_controller_system_update(BulletControllerSystem* self, EntityList* entities);

#endif