#ifndef RUNNER_UFO_CONTROLLER_COMPONENT_H
#define RUNNER_UFO_CONTROLLER_COMPONENT_H

#include "component.h"

typedef struct ufo_controller_component_t {
    Component super;
} UfoControllerComponent;

UfoControllerComponent* ufo_controller_component_new(Entity entity);
COMPONENT_DEFINE(COMPONENT_UFO_CONTROLLER);

#endif