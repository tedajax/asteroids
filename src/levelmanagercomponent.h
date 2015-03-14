#ifndef ASTEROIDS_LEVEL_MANAGER_COMPONENT_H
#define ASTEROIDS_LEVEL_MANAGER_COMPONENT_H

#include "core.h"
#include "component.h"

typedef struct level_manager_component_t {
    Component super;
    LevelManagerConfig* config;
    i32 currentLevel;
    i32 asteroidCount;
    i32 metalAsteroidCount;
    i32 smallUfoCount;
    i32 largeUfoCount;
    i32 livesRemaining;
} LevelManagerComponent;

LevelManagerComponent* level_manager_component_new(Entity entity, LevelManagerConfig* config);
COMPONENT_DEFINE(COMPONENT_LEVEL_MANAGER);

#endif