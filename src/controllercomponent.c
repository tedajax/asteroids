#include "controllercomponent.h"
#include "config.h"

ControllerComponent* controller_component_new(Entity entity, Config* config, char* section) {
    ControllerComponent* self = (ControllerComponent*)calloc(1, sizeof(ControllerComponent));

    component_init((Component*)self, COMPONENT_CONTROLLER, sizeof(ControllerComponent), entity);

    self->turnSpeed = config_get_float(config, section, "turn_speed");
    self->acceleration = config_get_float(config, section, "acceleration");
    self->maxSpeed = config_get_float(config, section, "max_speed");
    self->damageRecoveryInterval = config_get_float(config, section, "recovery_interval");
    self->currentSpeed = 0.f;
    self->damageRecoveryTimer = 0.f;

    //TODO get rid of these ini calls
    self->bulletSourceCount = ini_get_array_count(&config->data, section, "bullet_sources");
    self->bulletSources = CALLOC(self->bulletSourceCount, BulletSource);

    for (u32 i = 0; i < self->bulletSourceCount; ++i) {
        bullet_source_init(&self->bulletSources[i], config_get_BulletSourceConfig_at(config, section, "bullet_sources", i));
    }

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_CONTROLLER) {
    return (Component*)controller_component_new(0, config, (char*)table);
}

COMPONENT_FREE(COMPONENT_CONTROLLER) {
    ControllerComponent* self = (ControllerComponent*)component;
    for (u32 i = 0; i < self->bulletSourceCount; ++i) {
        bullet_source_release(&self->bulletSources[i]);
    }
    free(self->bulletSources);
}

COMPONENT_COPY(COMPONENT_CONTROLLER) {
    //TODO
}