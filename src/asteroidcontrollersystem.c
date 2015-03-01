#include "asteroidcontrollersystem.h"
#include "entityfactory.h"

void asteroid_controller_system_init(AsteroidControllerSystem* self, EntityManager* entityManager, Config* config, const char* table) {
    aspect_system_init(&self->super, entityManager, COMPONENT_ASTEROID_CONTROLLER, MAX_ENTITIES);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED, asteroid_controller_system_on_entity_removed);

    self->maxSize = CONFIG_GET(i32)(config, table, "max_size");
    self->speedMultiplier = CONFIG_GET(f32)(config, table, "speed_multiplier");
}

void asteroid_controller_system_start(AsteroidControllerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(GET_ENTITY(i), COMPONENT_TRANSFORM);

        transform->position.x = randf_range(0.f, (f32)globals.world.width);
        transform->position.y = randf_range(0.f, (f32)globals.world.height);
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

        if (is_approx_zero(vec2_length(&movement->velocity))) {
            f32 speed = (self->maxSize - asteroid->asteroidSize + 1) * self->speedMultiplier;
            movement->velocity.x = randf_range(-speed, speed);
            movement->velocity.y = randf_range(-speed, speed);
        }

        f32 scale = (f32)asteroid->asteroidSize / (f32)self->maxSize;
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
            Entity e = prefab_instantiate_at(prefab_get(asteroid->asteroidPrefabName), self->super.entityManager, transform->position, 0.f);
            AsteroidControllerComponent* a = (AsteroidControllerComponent*)entities_get_component(
                system->entityManager,
                COMPONENT_ASTEROID_CONTROLLER,
                e);
            a->asteroidSize = asteroid->asteroidSize - 1;
        }
    }
}