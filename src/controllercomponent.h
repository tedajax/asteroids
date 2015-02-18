#ifndef RUNNER_CONTROLLER_COMPONENT_H
#define RUNNER_CONTROLLER_COMPONENT_H

#include "core.h"
#include "component.h"
#include "bulletsource.h"

typedef struct controller_component_t {
    Component super;
    f32 turnSpeed;
    f32 acceleration;
    f32 maxSpeed;
    f32 currentSpeed;
    f32 damageRecoveryInterval;
    BulletSource* bulletSources;
    u32 bulletSourceCount;
    f32 damageRecoveryTimer;
} ControllerComponent;

ControllerComponent* controller_component_new(Entity entity, Config* config, char* section);
COMPONENT_DEFINE(COMPONENT_CONTROLLER);

#endif