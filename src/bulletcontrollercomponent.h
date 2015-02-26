#ifndef RUNNER_BULLET_CONTROLLER_COMPONENT_H
#define RUNNER_BULLET_CONTROLLER_COMPONENT_H

#include "core.h"
#include "component.h"

typedef struct bullet_controller_component_t {
    Component super;
    BulletConfig* configSource;
    BulletConfig config;
    dynf32 speed;
    dynf32 angle;
    f32 baseAngle;
    f32 lifeTimer;
    bool destroyFlag;
} BulletControllerComponent;

BulletControllerComponent* bullet_controller_component_new(Entity entity, BulletConfig* config);
void bullet_controller_component_set_config(BulletControllerComponent* self, BulletConfig* config);
COMPONENT_DEFINE(COMPONENT_BULLET_CONTROLLER);

#endif