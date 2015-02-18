#include "asteroidcontrollersystem.h"
#include "entityfactory.h"

void asteroid_controller_system_init(AsteroidControllerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_ASTEROID_CONTROLLER, 128);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED, asteroid_controller_system_on_entity_removed);
}

void asteroid_controller_system_update(AsteroidControllerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);
        AsteroidControllerComponent* asteroid =
            (AsteroidControllerComponent*)GET_SYSTEM_COMPONENT(i);

        MovementComponent* movement =
            (MovementComponent*)GET_COMPONENT(entity, COMPONENT_MOVEMENT);

        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        ColliderComponent* collider =
            (ColliderComponent*)GET_COMPONENT(entity, COMPONENT_COLLIDER);

        REQUIRED_COMPONENTS(asteroid, movement, transform);

        if (is_approx_zero(vec2_length(&movement->velocity))) {
            f32 speed = (5 - asteroid->asteroidSize + 1) * 75.f;
            movement->velocity.x = randf_range(-speed, speed);
            movement->velocity.y = randf_range(-speed, speed);
        }

        f32 scale = asteroid->asteroidSize / 5.f;
        vec2_set(&transform->scale, scale, scale);

        // TODO: Collision system should handle the scaling from the transform so this shouldn't be necessary
        BoundingCircle* circle = (BoundingCircle*)collider->collider.volume;
        circle->radius = 48.f * scale;
    }
}

void asteroid_controller_system_on_entity_removed(AspectSystem* system, Entity entity, const Message msg) {
    AsteroidControllerSystem* self = (AsteroidControllerSystem*)system;

    AsteroidControllerComponent* asteroid =
        (AsteroidControllerComponent*)GET_COMPONENT(entity, COMPONENT_ASTEROID_CONTROLLER);
    
    if (!asteroid) { return; }

    TransformComponent* transform =
        (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

    if (asteroid->asteroidSize > 1) {
        for (int i = 0; i < 2; ++i) {
            entity_create_asteroid(self->super.entityManager, transform->position, asteroid->asteroidSize - 1);
        }
    }
}