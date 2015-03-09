#ifndef ASTEROIDS_CONTROLLER_SYSTEM_H
#define ASTEROIDS_CONTROLLER_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "movementcomponent.h"
#include "controllercomponent.h"

typedef struct controller_system_t {
    AspectSystem super;
} ControllerSystem;

void controller_system_init(ControllerSystem* self, EntityManager* entityManager);
void controller_system_update(ControllerSystem* self);
void controller_system_on_collision_enter(AspectSystem* system, Entity entity, Message message);
void controller_system_on_damage(AspectSystem* system, Entity entity, Message message);

#endif