#include "entityfactory.h"

Entity entity_create_player(EntityManager* entityManager, Vec2 position) {
    return prefab_instantiate_at(prefab_get("player_ship.prefab"), entityManager, position, 0.f);
}

Entity entity_create_bullet(EntityManager* entityManager, Vec2 position, f32 baseAngle) {
    Entity entity = prefab_instantiate_at(prefab_get("player_bullet.prefab"), entityManager, position, 0.f);

    BulletControllerComponent* bullet =
        (BulletControllerComponent*)entities_get_component(entityManager,
        COMPONENT_BULLET_CONTROLLER,
        entity);

    bullet->baseAngle = baseAngle;

    return entity;
}

Entity entity_create_bg_manager(EntityManager* entityManager) {
    return prefab_instantiate(prefab_get("background.prefab"), entityManager);
}

Entity entity_create_basic_enemy(EntityManager* entityManager, Vec2 position) {
    Entity entity = entities_create_entity(entityManager);

    entities_add_component(entityManager,
        (Component*)transform_component_new(entity, position, 0.f, vec2_one()));

    entities_add_component(entityManager,
        (Component*)movement_component_new(entity, vec2_zero(), 0.f));

    //entities_add_component(entityManager,
    //    (Component*)enemy_component_new(entity, ENEMY_TYPE_BASIC));

    entities_add_component(entityManager,
        (Component*)health_component_new(entity, 100));

    entities_add_component(entityManager,
        (Component*)sprite_component_new(entity, atlas_get("atlas1"), "enemy_red_1", 1));

    Collider collider;
    collider_init_bcircle(&collider,
        entity,
        COLLIDER_LAYER_ENEMY,
        vec2_zero(),
        64.f);

    entities_add_component(entityManager,
        (Component*)collider_component_new(entity, &collider));

    entities_add_component(entityManager,
        (Component*)screen_wrap_component_new(entity, 64, 64));

    return entity;
}

Entity entity_create_asteroid(EntityManager* entityManager, char* prefabName) {
    Prefab* prefab = prefab_get(prefabName);
    ASSERT(prefab, "");

    Entity entity = prefab_instantiate(prefab, entityManager);

    AsteroidControllerComponent* asteroid =
        (AsteroidControllerComponent*)entities_get_component(entityManager,
        COMPONENT_ASTEROID_CONTROLLER,
        entity);

    asteroid->asteroidPrefabName = prefabName;

    return entity;
}