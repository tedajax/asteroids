#ifndef RUNNER_UFO_CONTROLLER_SYSTEM_H
#define RUNNER_UFO_CONTROLLER_SYSTEM_H

#include "aspectsystem.h"

typedef struct ufo_controller_system_t {
    AspectSystem super;
} UfoControllerSystem;

void ufo_controller_system_init(UfoControllerSystem* self, EntityManager* entityManager);
void ufo_controller_system_update(UfoControllerSystem* self);

#endif