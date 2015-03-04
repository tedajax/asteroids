#include "ufocontrollersystem.h"
#include "ufocontrollercomponent.h"
#include "movementcomponent.h"
#include "messaging.h"

void ufo_controller_system_init(UfoControllerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_UFO_CONTROLLER, 16);

    REGISTER_SYSTEM_HANDLER(MESSAGE_GENERIC_TICK, ufo_controller_system_on_tick);
}

void ufo_controller_system_update(UfoControllerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);

        UfoControllerComponent* ufo =
            (UfoControllerComponent*)GET_SYSTEM_COMPONENT(i);

        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        MovementComponent* movement =
            (MovementComponent*)GET_COMPONENT(entity, COMPONENT_MOVEMENT);
        
        REQUIRED_COMPONENTS(ufo, movement);

        movement->velocity.x = 100.f;
        movement->velocity.y = 30.f;

        if (!ufo->fireTimer) {
            Message msg;
            msg.type = MESSAGE_GENERIC_TICK;
            ufo->fireTimer = timer_add_interval(entity, msg, 0.f, 1.f);
        }

        switch (ufo->ufoType) {
            default:
            case UFO_BIG:
                break;

            case UFO_SMALL:
                break;
        }

        if (input_key_down(SDL_SCANCODE_E)) {
            Vec2 direction = { 1.f, 0.f };
            bullet_source_fire_direction(&ufo->bulletSource,
                self->super.entityManager,
                transform,
                &direction);
        }
    }
}

void ufo_controller_system_on_tick(AspectSystem* system, Entity entity, const Message msg) {
    UfoControllerComponent* ufo =
        (UfoControllerComponent*)entities_get_component(system->entityManager, COMPONENT_UFO_CONTROLLER, entity);

    TransformComponent* tx =
        (TransformComponent*)entities_get_component(system->entityManager, COMPONENT_TRANSFORM, entity);

    Vec2 fireDirection = vec2_unit_x();

    switch (ufo->ufoType) {
        default:
        case UFO_BIG:
            fireDirection = vec2_rand_direction();
            break;

        case UFO_SMALL: {
            Entity player = entities_get_named_entity(system->entityManager, "player");
            if (player) {
                TransformComponent* playerTx =
                    (TransformComponent*)entities_get_component(system->entityManager, COMPONENT_TRANSFORM, player);

                vec2_sub(&playerTx->position, &tx->position, &fireDirection);
                vec2_normalize(&fireDirection, &fireDirection);
                f32 angle = vec2_direction_angle(&fireDirection);
                angle += randf_range(-30.f, 30.f);
                vec2_direction(angle, &fireDirection);
            }
            break;
        }
    }


    bullet_source_fire_direction(&ufo->bulletSource, system->entityManager, tx, &fireDirection);
}