#include "ufocontrollersystem.h"
#include "ufocontrollercomponent.h"
#include "movementcomponent.h"
#include "messaging.h"

void ufo_controller_system_init(UfoControllerSystem* self, GameScene* scene) {
    aspect_system_init(&self->super, scene, COMPONENT_UFO_CONTROLLER, 16);

    REGISTER_SYSTEM_HANDLER(MESSAGE_GENERIC_TICK, ufo_controller_system_on_tick);
    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED, ufo_controller_system_on_remove);
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

        vec2_direction(ufo->movementAngle, &movement->velocity);
        vec2_scale(&movement->velocity, ufo->movementSpeed, &movement->velocity);

        if (!ufo->fireTimer) {
            Message msg;
            msg.type = MESSAGE_GENERIC_TICK;
            MessageOnTick params = { 0 };
            params.tickFlags = UFO_TICK_FIRE_BULLET;
            MESSAGE_SET_PARAM_BLOCK(msg, params);
            ufo->fireTimer = timer_add_interval(entity, msg, 0.f, 1.f);
        }

        if (!ufo->changeDirectionTimer) {
            Message msg;
            msg.type = MESSAGE_GENERIC_TICK;
            MessageOnTick params = { 0 };
            params.tickFlags = UFO_TICK_CHANGE_DIRECTION;
            MESSAGE_SET_PARAM_BLOCK(msg, params);
            ufo->changeDirectionTimer = timer_add_interval(entity, msg, 0.f, 1.f);
        }

        if (input_key_down(self->super.input, SDL_SCANCODE_E)) {
            Vec2 direction = { 1.f, 0.f };
            bullet_source_fire_direction(&ufo->bulletSource,
                self->super.entityManager,
                transform,
                &direction);
        }
    }
}

void ufo_tick_fire_bullet(UfoControllerSystem* self, Entity entity, Timer* timer);
void ufo_tick_change_direction(UfoControllerSystem* self, Entity entity, Timer* timer);

void ufo_controller_system_on_tick(AspectSystem* system, Entity entity, const Message msg) {
    UfoControllerSystem* self = (UfoControllerSystem*)system;
    
    MessageOnTick params;
    MESSAGE_GET_PARAM_BLOCK(msg, params);

    if ((params.tickFlags & UFO_TICK_FIRE_BULLET)) {
        ufo_tick_fire_bullet(self, entity, params.timer);
    }

    if ((params.tickFlags & UFO_TICK_CHANGE_DIRECTION)) {
        ufo_tick_change_direction(self, entity, params.timer);
    }
}

void ufo_tick_fire_bullet(UfoControllerSystem* self, Entity entity, Timer* timer) {
    UfoControllerComponent* ufo =
        (UfoControllerComponent*)entities_get_component(self->super.entityManager, COMPONENT_UFO_CONTROLLER, entity);

    TransformComponent* tx =
        (TransformComponent*)entities_get_component(self->super.entityManager, COMPONENT_TRANSFORM, entity);

    if (!ufo || !tx) { return; }

    Vec2 fireDirection = vec2_unit_x();

    switch (ufo->ufoType) {
        default:
        case UFO_BIG:
            fireDirection = vec2_rand_direction();
            break;

        case UFO_SMALL: {
            Entity player = entities_get_named_entity(self->super.entityManager, "player");
            if (player) {
                TransformComponent* playerTx =
                    (TransformComponent*)entities_get_component(self->super.entityManager, COMPONENT_TRANSFORM, player);

                vec2_sub(&playerTx->position, &tx->position, &fireDirection);
                vec2_normalize(&fireDirection, &fireDirection);
                f32 angle = vec2_direction_angle(&fireDirection);
                angle += randf_range(-30.f, 30.f);
                vec2_direction(angle, &fireDirection);
            }
            break;
        }
    }

    timer->interval = randf_range(0.5f, 1.f);

    bullet_source_fire_direction(&ufo->bulletSource, self->super.entityManager, tx, &fireDirection);
}

void ufo_tick_change_direction(UfoControllerSystem* self, Entity entity, Timer* timer) {
    UfoControllerComponent* ufo =
        (UfoControllerComponent*)entities_get_component(self->super.entityManager, COMPONENT_UFO_CONTROLLER, entity);

    ufo->movementAngle += randf_range(-45.f, 45.f);
}

void ufo_controller_system_on_remove(AspectSystem* system, Entity entity, const Message msg) {
    UfoControllerComponent* ufo =
        (UfoControllerComponent*)entities_get_component(system->entityManager, COMPONENT_UFO_CONTROLLER, entity);

    if (ufo) {
        if (ufo->fireTimer) {
            timer_remove(ufo->fireTimer);
        }

        if (ufo->changeDirectionTimer) {
            timer_remove(ufo->changeDirectionTimer);
        }
    }
}