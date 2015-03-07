#ifndef RUNNER_UFO_CONTROLLER_SYSTEM_H
#define RUNNER_UFO_CONTROLLER_SYSTEM_H

#include "aspectsystem.h"

typedef enum ufo_system_tick_flags_e {
    UFO_TICK_FIRE_BULLET            = 0x1,
    UFO_TICK_CHANGE_DIRECTION       = 0x2,
} UfoSystemTickFlags;

typedef struct ufo_controller_system_t {
    AspectSystem super;
} UfoControllerSystem;

void ufo_controller_system_init(UfoControllerSystem* self, EntityManager* entityManager);
void ufo_controller_system_update(UfoControllerSystem* self);

void ufo_controller_system_on_tick(AspectSystem* system, Entity entity, const Message msg);
void ufo_controller_system_on_remove(AspectSystem* system, Entity entity, const Message msg);

#endif