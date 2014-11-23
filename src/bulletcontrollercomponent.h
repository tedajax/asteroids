#ifndef RUNNER_BULLET_CONTROLLER_COMPONENT_H
#define RUNNER_BULLET_CONTROLLER_COMPONENT_H

#include "core.h"
#include "component.h"

typedef struct bullet_controller_component_t {
    Component super;
    f32 speed;
    bool destroy;
} BulletControllerComponent;

BulletControllerComponent* bullet_controller_component_new(f32 speed);
void bullet_controller_on_collision_enter(Component* component, Message message);

#endif