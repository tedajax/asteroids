#ifndef ASTEROIDS_TRANSFORM_SYSTEM_H
#define ASTEROIDS_TRANSFORM_SYSTEM_H

#include "aspectsystem.h"
#include "transformcomponent.h"

typedef struct transform_system_t {
    AspectSystem super;
} TransformSystem;

void transform_system_init(TransformSystem* self, GameScene* scene);

#endif