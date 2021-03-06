#ifndef ASTEROIDS_HEALTH_SYSTEM_H
#define ASTEROIDS_HEALTH_SYSTEM_H

#include "aspectsystem.h"
#include "healthcomponent.h"

typedef struct health_system_t {
    AspectSystem super;
} HealthSystem;

void health_system_init(HealthSystem* self, GameScene* scene);
void health_system_update(HealthSystem* self);

void health_system_on_damage(AspectSystem* system, Entity entity, Message message);

#endif