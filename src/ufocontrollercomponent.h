#ifndef RUNNER_UFO_CONTROLLER_COMPONENT_H
#define RUNNER_UFO_CONTROLLER_COMPONENT_H

#include "component.h"
#include "transformcomponent.h"
#include "bulletsource.h"
#include "timer.h"

typedef enum ufo_type_e {
    UFO_BIG,
    UFO_SMALL,
} UfoType;

typedef struct ufo_controller_component_t {
    Component super;
    UfoType ufoType;
    TransformComponent* playerTransform;
    BulletSource bulletSource;
    f32 movementAngle;
    f32 movementSpeed;
    Timer* fireTimer;
    Timer* changeDirectionTimer;
} UfoControllerComponent;

UfoControllerComponent* ufo_controller_component_new(Entity entity, UfoType ufoType, f32 speed, BulletSourceConfig* bulletSourceConfig);
COMPONENT_DEFINE(COMPONENT_UFO_CONTROLLER);

UfoType ufo_type_parse(const char* str);

#endif