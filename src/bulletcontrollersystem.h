#ifndef ASTEROIDS_BULLET_CONTROLLER_SYSTEM_H
#define ASTEROIDS_BULLET_CONTROLLER_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "movementcomponent.h"
#include "bulletcontrollercomponent.h"

typedef struct bullet_controller_system_t {
    AspectSystem super;
} BulletControllerSystem;

void bullet_controller_system_init(BulletControllerSystem* self, GameScene* scene);
void bullet_controller_system_update(BulletControllerSystem* self);

void bullet_controller_system_on_collision_enter(AspectSystem* system, Entity entity, Message message);

#endif