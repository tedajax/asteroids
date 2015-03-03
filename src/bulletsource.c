#include "bulletsource.h"
#include "entitymanager.h"
#include "entityfactory.h"
#include "texturemanager.h"

void bullet_source_init(BulletSource* self, BulletSourceConfig* config) {
    self->configSource = config;
    self->active = false;
    self->fireTimer = 0.f;
    self->burstTimer = 0.f;
    self->burstsRemaining = 0;
    self->burstShotsRemaining = 0;
    self->level = 0;

    // TODO: This breaks live reloading of configs so this should get fixed to subscribe to updates to the source config
    bullet_source_config_copy(self->configSource, &self->config);
    
    dynf32_start(&self->config.burstShotDelay, &globals.tweens);
    dynf32_start(&self->config.spread, &globals.tweens);
    dynf32_start(&self->config.fireDelay, &globals.tweens);
    dynf32_start(&self->config.startAngle, &globals.tweens);

    self->prefab = prefab_get(self->config.bulletPrefabName);
}

void bullet_source_release(BulletSource* self) {
    dynf32_release(&self->config.burstShotDelay);
    dynf32_release(&self->config.spread);
    dynf32_release(&self->config.fireDelay);
    dynf32_release(&self->config.startAngle);
}

void bullet_source_update(BulletSource* self, f32 dt, EntityManager* entityManager, TransformComponent* anchor) {
    if (self->burstsRemaining <= 0) { return; }

    if (self->burstShotsRemaining > 0) {
        if (self->burstTimer > 0.f) {
            self->burstTimer -= dt;
        }

        if (self->burstTimer <= 0.f) {
            bullet_source_fire(self, entityManager, anchor);
            --self->burstShotsRemaining;
            if (self->burstShotsRemaining > 0) {
                self->burstTimer = dynf32_get(&self->config.burstShotDelay);
            } else {
                --self->burstsRemaining;
            }
        }
    } else {
        if (self->fireTimer > 0.f) {
            self->fireTimer -= dt;
        }

        if (!self->active) {
            return;
        }

        if (self->fireTimer <= 0.f) {
            self->fireTimer = dynf32_get(&self->config.fireDelay);
            self->burstShotsRemaining = self->config.burstShotCount;
        }
    }
}

void bullet_source_fire(BulletSource* self, EntityManager* entityManager, TransformComponent* anchor) {
    f32 x = self->config.offset.x;
    f32 y = self->config.offset.y;
    f32 theta = anchor->rotation * DEG_TO_RAD;
    Vec2 transformedOffset;
    transformedOffset.x = x * cosf(theta) - y * sinf(theta);
    transformedOffset.y = x * sinf(theta) + y * cosf(theta);
    for (i32 i = 0; i < self->config.count; ++i) {
        Vec2 pos;
        vec2_add(&anchor->position, &transformedOffset, &pos);
        f32 sa = anchor->rotation;
        sa += (i - (self->config.count / 2)) * dynf32_get(&self->config.spread);
        sa += dynf32_get(&self->config.startAngle);

        Entity entity = prefab_instantiate_at(self->prefab, entityManager, pos, 0);
        BulletControllerComponent* bullet =
            (BulletControllerComponent*)entities_get_component(entityManager, COMPONENT_BULLET_CONTROLLER, entity);
        bullet->baseAngle = sa;
    }
}

void bullet_source_fire_direction(BulletSource* self, EntityManager* entityManager, TransformComponent* anchor, Vec2* direction) {
    TransformComponent dummyTransform = *anchor;
    dummyTransform.rotation = vec2_direction_angle(direction);
    bullet_source_fire(self, entityManager, &dummyTransform);
}