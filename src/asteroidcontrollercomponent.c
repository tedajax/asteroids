#include "asteroidcontrollercomponent.h"

AsteroidControllerComponent* asteroid_controller_component_new(Entity entity, char* asteroidPrefabName, i32 size) {
    AsteroidControllerComponent* self = CALLOC(1, AsteroidControllerComponent);

    component_init((Component*)self, COMPONENT_ASTEROID_CONTROLLER, sizeof(AsteroidControllerComponent), entity);

    self->asteroidSize = size;
    self->asteroidPrefabName = asteroidPrefabName;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_ASTEROID_CONTROLLER) {
    i32 size = CONFIG_GET(int)(config, table, "asteroid_size");
    char* prefabName = CONFIG_TRY_GET(string)(config, table, "asteroid_prefab", "asteroid.prefab");
    return (Component*)asteroid_controller_component_new(0, prefabName, size);
}

COMPONENT_FREE(COMPONENT_ASTEROID_CONTROLLER) {}

COMPONENT_COPY(COMPONENT_ASTEROID_CONTROLLER) {}
