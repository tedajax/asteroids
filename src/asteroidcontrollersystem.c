#include "asteroidcontrollersystem.h"

void asteroid_controller_system_init(AsteroidControllerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_ASTEROID_CONTROLLER, 128);
}

void asteroid_controller_system_update(AsteroidControllerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);
        AsteroidControllerComponent* asteroid =
            (AsteroidControllerComponent*)GET_SYSTEM_COMPONENT(i);

        MovementComponent* movement =
            (MovementComponent*)GET_COMPONENT(entity, COMPONENT_MOVEMENT);

        REQUIRED_COMPONENTS(asteroid, movement);

        if (is_approx_zero(vec2_length(&movement->velocity))) {
            movement->velocity.x = randf_range(-100.f, 100.f);
            movement->velocity.y = randf_range(-100.f, 100.f);
        }
    }
}