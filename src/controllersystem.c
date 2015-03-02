#include "controllersystem.h"
#include "entityfactory.h"

void controller_system_init(ControllerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_CONTROLLER, 1);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ON_COLLISION_ENTER,
        controller_system_on_collision_enter);

    REGISTER_SYSTEM_HANDLER(MESSAGE_DAMAGE,
        controller_system_on_damage);
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

        ParticleComponent* particle =
            (ParticleComponent*)GET_COMPONENT(entity, COMPONENT_PARTICLE);

        REQUIRED_COMPONENTS(transform, movement, controller, particle);

        if (controller->damageRecoveryTimer > 0.f) {
            controller->damageRecoveryTimer -= globals.time.delta;
        }

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
            particle->emitters[0].spawnNewParticles = true;
        } else {
            if (vec2_length(&movement->velocity) > 0.f) {
                Vec2 currentDirection;
                vec2_normalize(&movement->velocity, &currentDirection);
                f32 angle = vec2_direction_angle(&currentDirection);
                f32 velocityMag = vec2_length(&movement->velocity);
                vec2_set_angle(&acceleration, angle, velocityMag * controller->friction * globals.time.delta);
            }
            particle->emitters[0].spawnNewParticles = false;
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

void controller_system_on_collision_enter(AspectSystem* system, Entity entity, Message message) {
    ControllerComponent* controller =
        (ControllerComponent*)entities_get_component(system->entityManager,
        COMPONENT_CONTROLLER,
        entity);

    REQUIRED_COMPONENTS(controller);

    if (controller->damageRecoveryTimer > 0.f) {
        return;
    }

    // Damage health on this controller
    {
        Message damageMsg;
        MessageOnDamageParams damageParams;
        damageParams.damage = 10;
        message_init(&damageMsg, MESSAGE_DAMAGE);
        MESSAGE_SET_PARAM_BLOCK(damageMsg, damageParams);

        Entity target = entity;

        entities_send_message(system->entityManager, target, damageMsg);
    }

    // Damage that thing that hit this controller
    {
        Message damageMsg;
        MessageOnDamageParams damageParams;
        damageParams.damage = 100000;
        damageParams.direction = vec2_init(0.f, 0.f);
        message_init(&damageMsg, MESSAGE_DAMAGE);
        MESSAGE_SET_PARAM_BLOCK(damageMsg, damageParams);

        MessageOnCollisionParams* params = (MessageOnCollisionParams*)message.paramBlock;
        Entity target = params->other;

        entities_send_message(system->entityManager, target, damageMsg);
    }
}

void controller_system_on_damage(AspectSystem* system, Entity entity, Message message) {
    ControllerComponent* controller =
        (ControllerComponent*)entities_get_component(system->entityManager,
        COMPONENT_CONTROLLER,
        entity);

    REQUIRED_COMPONENTS(controller);

    controller->damageRecoveryTimer = controller->damageRecoveryInterval;
}