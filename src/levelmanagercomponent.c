#include "levelmanagercomponent.h"

LevelManagerComponent* level_manager_component_new(Entity entity, LevelManagerConfig* config) {
    LevelManagerComponent* self = CALLOC(1, LevelManagerComponent);

    component_init((Component*)self, COMPONENT_LEVEL_MANAGER, sizeof(LevelManagerComponent), entity);

    self->config = config;
    self->currentLevel = 1;

    self->asteroidCount = 0;
    self->metalAsteroidCount = 0;
    self->smallUfoCount = 0;
    self->largeUfoCount = 0;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_LEVEL_MANAGER) {
    LevelManagerConfig* levelsConfig = CONFIG_GET(LevelManagerConfig)(config, table, "config");
    return (Component*)level_manager_component_new(0, levelsConfig);
}

COMPONENT_COPY(COMPONENT_LEVEL_MANAGER) { }
COMPONENT_FREE(COMPONENT_LEVEL_MANAGER) { }