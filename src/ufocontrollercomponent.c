#include "ufocontrollercomponent.h"

UfoControllerComponent* ufo_controller_component_new(Entity entity) {
    UfoControllerComponent* self = CALLOC(1, UfoControllerComponent);

    component_init(&self->super, COMPONENT_UFO_CONTROLLER, sizeof(UfoControllerComponent), entity);

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_UFO_CONTROLLER) {
    return (Component*)ufo_controller_component_new(0);
}

COMPONENT_FREE(COMPONENT_UFO_CONTROLLER) { }
COMPONENT_COPY(COMPONENT_UFO_CONTROLLER) { }