#include "bulletcontrollercomponent.h"

BulletControllerComponent* bullet_controller_component_new(Entity entity, BulletConfig* config) {
    BulletControllerComponent* self = (BulletControllerComponent*)calloc(1,
        sizeof(BulletControllerComponent));

    component_init((Component*)self, COMPONENT_BULLET_CONTROLLER, sizeof(BulletControllerComponent), entity);

    self->configSource = NULL;

    if (config) {
        bullet_controller_component_set_config(self, config);
    }

    return self;
}

void bullet_controller_component_set_config(BulletControllerComponent* self, BulletConfig* config) {
    ASSERT(self->configSource == NULL, "Attempt to configure an already configured bullet.");
    
    self->configSource = config;
    self->config = *config;
    self->speed = self->config.speed;
    self->angle = self->config.angle;
    self->baseAngle = 0.f;
    self->lifeTimer = self->config.lifetime;
    self->destroyFlag = false;

    dynf32_copy(&self->config.speed, &self->speed);
    dynf32_copy(&self->config.angle, &self->angle);

    dynf32_start(&self->speed, &globals.tweens);
    dynf32_start(&self->angle, &globals.tweens);
}

COMPONENT_DESERIALIZE(COMPONENT_BULLET_CONTROLLER) {
    BulletConfig* bulletConfig = CONFIG_TRY_GET(BulletConfig)(config, table, "config", NULL);
    return (Component*)bullet_controller_component_new(0, bulletConfig);
}

COMPONENT_FREE(COMPONENT_BULLET_CONTROLLER) {
    BulletControllerComponent* self = (BulletControllerComponent*)component;
    dynf32_release(&self->speed);
    dynf32_release(&self->angle);
}

COMPONENT_COPY(COMPONENT_BULLET_CONTROLLER) {}