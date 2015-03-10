#include "movementsystem.h"

void movement_system_init(MovementSystem* self, GameScene* scene) {
    aspect_system_init(&self->super, scene, COMPONENT_MOVEMENT, MAX_ENTITIES);
}

void movement_system_update(MovementSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);
        MovementComponent* movement =
            (MovementComponent*)GET_SYSTEM_COMPONENT(i);

        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        REQUIRED_COMPONENTS(transform, movement);

        Vec2 timeVel;
        vec2_scale(&movement->velocity, globals.time.delta, &timeVel);
        vec2_add(&transform->position, &timeVel, &transform->position);

        transform->rotation += movement->angularVelocity * globals.time.delta;

        if (movement->constrainToCamera) {
            log_error("Movement", "What does constrain to camera actually mean?");
        }
    }
}