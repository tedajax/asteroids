#include "bulletcontrollercomponent.h"

BulletControllerComponent* bullet_controller_component_new(BulletConfig* config) {
    BulletControllerComponent* self = (BulletControllerComponent*)calloc(1,
        sizeof(BulletControllerComponent));

    component_init((Component*)self, COMPONENT_BULLET_CONTROLLER);

    self->config = *config;
    self->speed = self->config.speed;
    self->angle = self->config.angle;
    self->baseAngle = self->config.baseAngle;
    self->lifeTimer = self->config.lifetime;
    self->destroy = false;

    dynf32_start_tween(&self->speed, &globals.tweens);
    dynf32_start_tween(&self->angle, &globals.tweens);

    return self;
}

void bullet_controller_on_collision_enter(Component* component, Message message) {
    BulletControllerComponent* self = (BulletControllerComponent*)component;
    self->destroy = true;
}