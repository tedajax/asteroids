#include "controllersystem.h"
#include "entityfactory.h"

void controller_system_init(ControllerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_CONTROLLER, 1);
}

void controller_system_update(ControllerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);
        ControllerComponent* controller = (ControllerComponent*)GET_SYSTEM_COMPONENT(i);

        MovementComponent* movement =
            (MovementComponent*)GET_COMPONENT(entity, COMPONENT_MOVEMENT);

        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        REQUIRED_COMPONENTS(transform, movement, controller);

        f32 turnRate = 0.f;
        Vec2 acceleration = vec2_zero();

        if (input_key(SDL_SCANCODE_LEFT)) {
            turnRate -= 1.f;
        }

        if (input_key(SDL_SCANCODE_RIGHT)) {
            turnRate += 1.f;
        }

        if (input_key(SDL_SCANCODE_UP)) {
            vec2_set_angle(&acceleration, transform->rotation, controller->acceleration * globals.time.delta);
        }

        turnRate *= controller->turnSpeed;

        vec2_add(&movement->velocity, &acceleration, &movement->velocity);
        if (vec2_length(&movement->velocity) > controller->maxSpeed) {
            vec2_normalize(&movement->velocity, &movement->velocity);
            vec2_scale(&movement->velocity, controller->maxSpeed, &movement->velocity);
        }

        movement->angularVelocity = turnRate;

        if (input_key_down(SDL_SCANCODE_S)) {
            movement->velocity = vec2_zero();
        }

        if (input_key_down(SDL_SCANCODE_Z)) {
            for (u32 i = 0; i < controller->bulletSourceCount; ++i) {
                bullet_source_on(&controller->bulletSources[i]);
            }
        }

        if (input_key_up(SDL_SCANCODE_Z)) {
            for (u32 i = 0; i < controller->bulletSourceCount; ++i) {
                bullet_source_off(&controller->bulletSources[i]);
            }
        }

        for (u32 i = 0; i < controller->bulletSourceCount; ++i) {
            bullet_source_update(&controller->bulletSources[i], globals.time.delta, self->super.entityManager, transform);
        }
    }
}
