#include "ufocontrollersystem.h"
#include "ufocontrollercomponent.h"

void ufo_controller_system_init(UfoControllerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_UFO_CONTROLLER, 16);
}

void ufo_controller_system_update(UfoControllerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);
        UfoControllerComponent* ufo =
            (UfoControllerComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(ufo);
    }
}