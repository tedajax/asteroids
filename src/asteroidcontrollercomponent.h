#ifndef RUNNER_ASTEROID_CONTROLLER_COMPONENT_H
#define RUNNER_ASTEROID_CONTROLLER_COMPONENT_H

#include "core.h"
#include "component.h"
#include "prefab.h"

typedef struct asteroid_component_t {
    Component super;
    i32 asteroidSize;
    char* asteroidPrefabName;
} AsteroidControllerComponent;

AsteroidControllerComponent* asteroid_controller_component_new(Entity entity, char* asteroidPrefabName, i32 size);
COMPONENT_DEFINE(COMPONENT_ASTEROID_CONTROLLER);

#endif
