#ifndef ASTEROIDS_GRAVITY_COMPONENT_H
#define ASTEROIDS_GRAVITY_COMPONENT_H

#include "core.h"
#include "component.h"
#include "movementcomponent.h"

typedef struct gravity_component_t {
    Component super;
    Vec2 gravAccel;
} GravityComponent;

GravityComponent* gravity_component_new(Entity entity, const Vec2* acceleration);
COMPONENT_DEFINE(COMPONENT_GRAVITY);

#endif