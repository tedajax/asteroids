#include "bulletcontrollersystem.h"
#include "collidercomponent.h"
#include "physics.h"
#include "prefab.h"

void bullet_controller_system_init(BulletControllerSystem* self, GameScene* scene) {
    aspect_system_init(&self->super, scene, COMPONENT_BULLET_CONTROLLER, 512);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ON_COLLISION_ENTER, bullet_controller_system_on_collision_enter);
}

void bullet_controller_system_update(BulletControllerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);
        BulletControllerComponent* bullet = (BulletControllerComponent*)GET_SYSTEM_COMPONENT(i);

        TransformComponent* transform = (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        MovementComponent* movement = (MovementComponent*)GET_COMPONENT(entity, COMPONENT_MOVEMENT);

        REQUIRED_COMPONENTS(transform, movement, bullet);

        transform->position.x += globals.scrollSpeed * globals.time.delta;

        f32 speed = dynf32_get(&bullet->speed);
        f32 angle = bullet->baseAngle + dynf32_get(&bullet->angle);

        transform->rotation = angle;

        f32 bulletRadAngle = angle * DEG_TO_RAD;
        vec2_set(&movement->velocity, speed * cosf(bulletRadAngle),
            speed * sinf(bulletRadAngle));

        bullet->lifeTimer -= globals.time.delta;

        if (bullet->lifeTimer <= 0) {
            bullet->destroyFlag = true;
        }

        if (bullet->destroyFlag) {
            prefab_instantiate_at(prefab_get("bullet_explosion.prefab"), self->super.entityManager, transform->position, 0.f);
            entities_remove_entity(self->super.entityManager, entity);
        }
    }
}

void bullet_controller_system_on_collision_enter(AspectSystem* system, Entity entity, Message message) {
    BulletControllerComponent* bullet =
        (BulletControllerComponent*)entities_get_component(system->entityManager,
        COMPONENT_BULLET_CONTROLLER,
        entity);

    MovementComponent* movement =
        (MovementComponent*)entities_get_component(system->entityManager, COMPONENT_MOVEMENT, entity);

    REQUIRED_COMPONENTS(bullet);

    bullet->destroyFlag = true;

    i32 damage = bullet->config.damage;
    Vec2 direction;
    vec2_normalize(&movement->velocity, &direction);

    Message damageMsg;
    MessageOnDamageParams damageParams;
    damageParams.damage = damage;
    damageParams.direction = direction;
    message_init(&damageMsg, MESSAGE_DAMAGE);
    MESSAGE_SET_PARAM_BLOCK(damageMsg, damageParams);

    MessageOnCollisionParams* params = (MessageOnCollisionParams*)message.paramBlock;
    Entity target = params->other;

    entities_send_message(system->entityManager, target, damageMsg);
}
