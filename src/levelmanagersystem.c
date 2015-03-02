#include "levelmanagersystem.h"
#include "asteroidcontrollercomponent.h"

void level_manager_system_init(LevelManagerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_LEVEL_MANAGER, 1);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_ADDED, level_manager_on_entity_added);
    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED, level_manager_on_entity_removed);
}

void level_manager_system_start(LevelManagerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    ComponentList* asteroidList = entities_get_all_components(self->super.entityManager, COMPONENT_ASTEROID_CONTROLLER);

    for (u32 i = 0; i < components->count; ++i) {
        entities_subscribe(self->super.entityManager, MESSAGE_ENTITY_ADDED, GET_ENTITY(i));
        entities_subscribe(self->super.entityManager, MESSAGE_ENTITY_REMOVED, GET_ENTITY(i));
        
        LevelManagerComponent* levelManager =
            (LevelManagerComponent*)GET_SYSTEM_COMPONENT(i);
        
        // will need to be updated to count differences between metal/regular asteroids
        levelManager->asteroidCount = asteroidList->count;
    }
}

void level_manager_system_update(LevelManagerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        // TBD...
    }
}

void level_manager_update_asteroid_count(EntityManager* entityManager, LevelManagerComponent* self) {
    ComponentList* asteroidList = entities_get_all_components(entityManager, COMPONENT_ASTEROID_CONTROLLER);
    u32 old = self->asteroidCount;
    self->asteroidCount = asteroidList->count;
    if (self->asteroidCount != old) {
        log_info_format("Level", "Asteroids remaining: %d", self->asteroidCount);
    }
}

void level_manager_on_entity_added(AspectSystem* system, Entity entity, const Message msg) {
    LevelManagerSystem* self = (LevelManagerSystem*)system;

    LevelManagerComponent* levelManager =
        (LevelManagerComponent*)entities_get_component(system->entityManager,
        COMPONENT_LEVEL_MANAGER,
        entity);

    if (levelManager) {
        level_manager_update_asteroid_count(self->super.entityManager, levelManager);
    }
}

void level_manager_on_entity_removed(AspectSystem* system, Entity entity, const Message msg) {
    LevelManagerSystem* self = (LevelManagerSystem*)system;

    LevelManagerComponent* levelManager =
        (LevelManagerComponent*)entities_get_component(system->entityManager,
        COMPONENT_LEVEL_MANAGER,
        entity);

    level_manager_update_asteroid_count(self->super.entityManager, levelManager);
}
