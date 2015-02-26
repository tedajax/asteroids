#include "controllercomponent.h"
#include "config.h"

ControllerComponent* controller_component_new(Entity entity, Config* config, char* section) {
    ControllerComponent* self = (ControllerComponent*)calloc(1, sizeof(ControllerComponent));

    component_init((Component*)self, COMPONENT_CONTROLLER, sizeof(ControllerComponent), entity);

    self->turnSpeed = config_get_float(config, section, "turn_speed");
    self->acceleration = config_get_float(config, section, "acceleration");
    self->maxSpeed = config_get_float(config, section, "max_speed");
    self->friction = config_get_float(config, section, "friction");
    self->damageRecoveryInterval = config_get_float(config, section, "recovery_interval");
    self->currentSpeed = 0.f;
    self->damageRecoveryTimer = 0.f;

    //TODO get rid of these ini calls
    self->bulletSourceCount = ini_get_array_count(&config->data, section, "bullet_sources");
    self->bulletSources = CALLOC(self->bulletSourceCount, BulletSource);

    for (u32 i = 0; i < self->bulletSourceCount; ++i) {
        bullet_source_init(&self->bulletSources[i], CONFIG_GET_AT(BulletSourceConfig)(config, section, "bullet_sources", i));
    }

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_CONTROLLER) {
    char* configPath = CONFIG_GET(string)(config, table, "config_path");
    
    char configName[128];
    config_extract_config_file(configPath, configName, 128);

    char sectionName[128];
    config_extract_section(configPath, sectionName, 128);

    Config* cfg = config_get(configName);

    ASSERT(cfg, "Could not find specified config file.");

    return (Component*)controller_component_new(0, cfg, (char*)sectionName);
}

COMPONENT_FREE(COMPONENT_CONTROLLER) {
    ControllerComponent* self = (ControllerComponent*)component;
    for (u32 i = 0; i < self->bulletSourceCount; ++i) {
        bullet_source_release(&self->bulletSources[i]);
    }
    free(self->bulletSources);
}

COMPONENT_COPY(COMPONENT_CONTROLLER) {
    ControllerComponent* sourceController = (ControllerComponent*)source;
    ControllerComponent* destController = (ControllerComponent*)dest;

    destController->bulletSources = CALLOC(sourceController->bulletSourceCount, BulletSource);


}