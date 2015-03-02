#include "asteroidcontrollersystem.h"
#include "entityfactory.h"

f32 get_speed(i32 maxSize, i32 size, f32 speedMult) {
    return (maxSize - size + 1) * speedMult;
}

void asteroid_controller_system_init(AsteroidControllerSystem* self, EntityManager* entityManager, Config* config, const char* table) {
    aspect_system_init(&self->super, entityManager, COMPONENT_ASTEROID_CONTROLLER, MAX_ENTITIES);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED, asteroid_controller_system_on_entity_removed);
    REGISTER_SYSTEM_HANDLER(MESSAGE_DAMAGE, asteroid_controller_system_on_damage);

    self->maxSize = CONFIG_GET(i32)(config, table, "max_size");
    self->normalSpeedMultiplier = CONFIG_GET(f32)(config, table, "normal_speed_multiplier");
    self->maxSpeedMultiplier = CONFIG_GET(f32)(config, table, "max_speed_multiplier");
    self->friction = CONFIG_GET(f32)(config, table, "friction");
}

void asteroid_controller_system_start(AsteroidControllerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(GET_ENTITY(i), COMPONENT_TRANSFORM);

        transform->position.x = randf_range(0.f, (f32)globals.world.width);
        transform->position.y = randf_range(0.f, (f32)globals.world.height);

        MovementComponent* movement =
            (MovementComponent*)GET_COMPONENT(GET_ENTITY(i), COMPONENT_MOVEMENT);

        AsteroidControllerComponent* asteroid =
            (AsteroidControllerComponent*)GET_COMPONENT(GET_ENTITY(i), COMPONENT_ASTEROID_CONTROLLER);

        f32 speed = get_speed(self->maxSize, asteroid->size, self->normalSpeedMultiplier);
        vec2_set_angle(&movement->velocity, randf(360.f), speed);
    }
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

        f32 normalSpeed = get_speed(self->maxSize, asteroid->size, self->normalSpeedMultiplier);
        f32 maxSpeed = get_speed(self->maxSize, asteroid->size, self->maxSpeedMultiplier);

        f32 currentSpeed = vec2_length(&movement->velocity);
        if (currentSpeed > normalSpeed) {
            Vec2 currentDirection;
            vec2_normalize(&movement->velocity, &currentDirection);
            f32 angle = vec2_direction_angle(&currentDirection);
            f32 velocityMag = vec2_length(&movement->velocity);
            Vec2 acceleration;
            vec2_set_angle(&acceleration, angle, velocityMag * self->friction * globals.time.delta);
            vec2_add(&movement->velocity, &acceleration, &movement->velocity);
        }

        f32 scale = (f32)asteroid->size / (f32)self->maxSize;
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

    if (asteroid->size > 1) {
        for (int i = 0; i < 2; ++i) {
            Entity e = prefab_instantiate_at(prefab_get(asteroid->prefabName), self->super.entityManager, transform->position, 0.f);
            
            AsteroidControllerComponent* a = (AsteroidControllerComponent*)entities_get_component(
                system->entityManager,
                COMPONENT_ASTEROID_CONTROLLER,
                e);

            a->size = asteroid->size - 1;

            MovementComponent* movement =
                (MovementComponent*)entities_get_component(system->entityManager, COMPONENT_MOVEMENT, e);

            f32 speed = get_speed(self->maxSize, a->size, self->maxSpeedMultiplier);
            f32 angle = asteroid->lastHitAngle + (30.f * powf(-1, (i + 1))) + 180.f;
            vec2_set_angle(&movement->velocity, angle + (30.f * powf(-1.f, (i + 1))), speed);
        }
    }
}

void asteroid_controller_system_on_damage(AspectSystem* system, Entity entity, const Message msg) {
    MessageOnDamageParams params;
    MESSAGE_GET_PARAM_BLOCK(msg, params);

    AsteroidControllerComponent* asteroid = (AsteroidControllerComponent*)entities_get_component(system->entityManager, COMPONENT_ASTEROID_CONTROLLER, entity);
    TransformComponent* tx = (TransformComponent*)entities_get_component(system->entityManager, COMPONENT_TRANSFORM, entity);
    MovementComponent* movement = (MovementComponent*)entities_get_component(system->entityManager, COMPONENT_MOVEMENT, entity);
   
    Vec2 force;
    vec2_scale(&params.direction, 50.f, &force);
    asteroid->lastHitAngle = vec2_direction_angle(&params.direction);
    vec2_add(&force, &movement->velocity, &movement->velocity);
}
