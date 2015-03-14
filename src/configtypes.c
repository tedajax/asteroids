#include "configtypes.h"
#include "collider.h"
#include "config.h"

void collider_config_deserialize(TypeConfig* super, Config* config, char* table) {
    ColliderConfig* self = (ColliderConfig*)super;

    self->layer = collider_layer_parse(config_get_string(config, table, "layer"));
    self->offset = config_get_Vec2(config, table, "offset");

    char* volumeStr = config_try_get_string(config, table, "type", NULL);
    self->type = BOUNDING_VOLUME_AA_BOX;
    if (volumeStr) {
        self->type = bounding_volume_type_parse(volumeStr);
    }

    switch (self->type) {
    	default:
    	case BOUNDING_VOLUME_AA_BOX:
            self->width = config_get_float(config, table, "width");
            self->height = config_get_float(config, table, "height");
    		break;

    	case BOUNDING_VOLUME_O_BOX:
            self->width = config_get_float(config, table, "width");
            self->height = config_get_float(config, table, "height");
            self->orientation = config_try_get_float(config, table, "orientation", 0.f);
    		break;

    	case BOUNDING_VOLUME_CIRCLE:
            self->radius = config_get_float(config, table, "radius");
    		break;
    }
}

ColliderLayer collider_layer_parse(char* str) {
    if (STR_ENUM_CMP(str, COLLIDER_LAYER_PLAYER)) {
        return COLLIDER_LAYER_PLAYER;
    } else if (STR_ENUM_CMP(str, COLLIDER_LAYER_ENEMY)) {
        return COLLIDER_LAYER_ENEMY;
    } else if (STR_ENUM_CMP(str, COLLIDER_LAYER_ENVIRONMENT)) {
        return COLLIDER_LAYER_ENVIRONMENT;
    } else if (STR_ENUM_CMP(str, COLLIDER_LAYER_PLAYER_PROJECTILE)) {
        return COLLIDER_LAYER_PLAYER_PROJECTILE;
    } else if (STR_ENUM_CMP(str, COLLIDER_LAYER_ENEMY_PROJECTILE)) {
        return COLLIDER_LAYER_ENEMY_PROJECTILE;
    } else if (STR_ENUM_CMP(str, COLLIDER_LAYER_LAST)) {
        return COLLIDER_LAYER_LAST;
    } else {
        ASSERT(false, "Unable to parse ColliderLayer enum.");
        return 0;
    }
}

void bullet_source_config_deserialize(TypeConfig* super, Config* config, char* table) {
    BulletSourceConfig* self = (BulletSourceConfig*)super;

    self->offset = config_get_Vec2(config, table, "offset");
    self->count = config_get_int(config, table, "count");
    self->burstShotCount = config_try_get_int(config, table, "burst_shot_count", 1);
    self->burstCount = config_try_get_int(config, table, "burst_count", INT32_MAX);
    dynf32 defaultBurstDelay = { 0.f, DYN_F32_VALUE, NULL, NULL };
    self->burstShotDelay = config_try_get_dynf32(config, table, "burst_shot_delay", defaultBurstDelay);
    self->spread = config_get_dynf32(config, table, "spread");
    self->fireDelay = config_get_dynf32(config, table, "fire_delay");
    self->startAngle = config_get_dynf32(config, table, "start_angle");
    self->bulletPrefabName = CONFIG_GET(string)(config, table, "prefab");
}

void bullet_source_config_copy(const BulletSourceConfig* source, BulletSourceConfig* dest) {
    memcpy(dest, source, sizeof(BulletSourceConfig));
    dynf32_copy(&source->burstShotDelay, &dest->burstShotDelay);
    dynf32_copy(&source->spread, &dest->spread);
    dynf32_copy(&source->fireDelay, &dest->fireDelay);
    dynf32_copy(&source->startAngle, &dest->startAngle);
}

void bullet_config_deserialize(TypeConfig* super, Config* config, char* table) {
    BulletConfig* self = (BulletConfig*)super;
    self->lifetime = CONFIG_GET(float)(config, table, "lifetime");
    self->speed = CONFIG_GET(dynf32)(config, table, "speed");
    self->angle = CONFIG_GET(dynf32)(config, table, "angle");
    self->damage = (i32)CONFIG_GET(int)(config, table, "damage");
}

void bullet_config_copy(const BulletConfig* source, BulletConfig* dest) {
    memcpy(dest, source, sizeof(BulletConfig));
    dynf32_copy(&source->speed, &dest->speed);
    dynf32_copy(&source->angle, &dest->angle);
}

void tween_config_deserialize(TypeConfig* super, Config* config, char* table) {
    TweenConfig* self = (TweenConfig*)super;

    char* tweenFuncName = config_get_string(config, table, "function");
    self->function = tween_parse(tweenFuncName);
    self->start = CONFIG_GET(RandomRange)(config, table, "start");
    self->end = CONFIG_GET(RandomRange)(config, table, "end");
    self->duration = config_get_float(config, table, "duration");
    self->timescale = config_try_get_float(config, table, "timescale", 1.f);
    self->loops = config_try_get_int(config, table, "loops", TWEEN_LOOP_INFINITE);
}

void particle_emitter_config_deserialize(TypeConfig* super, Config* config, char* table) {
    ParticleEmitterConfig* self = (ParticleEmitterConfig*)super;

    self->atlasName = CONFIG_GET(string)(config, table, "texture_atlas");
    self->spriteName = CONFIG_GET(string)(config, table, "sprite");
    self->lifetime = CONFIG_GET(float)(config, table, "lifetime");
    self->maxParticles = (u32)CONFIG_GET(int)(config, table, "max_particles");
    self->particlesPerEmission = (u32)CONFIG_GET(int)(config, table, "emission_count");
    self->particleLifetime = CONFIG_GET(float)(config, table, "particle_lifetime");
    self->emissionInterval = CONFIG_GET(float)(config, table, "emission_interval");
    self->emissionBaseAngle = CONFIG_TRY_GET(float)(config, table, "base_angle", 0.f);
    self->emissionArcLength = CONFIG_TRY_GET(float)(config, table, "arc_length", 360.f);
    self->startingRotation = CONFIG_TRY_GET(dynf32)(config, table, "starting_rotation", dynf32_value(0.f));
    self->rotationSpeed = CONFIG_TRY_GET(dynf32)(config, table, "rotation_speed", dynf32_value(0.f));
    self->speed = CONFIG_TRY_GET(dynf32)(config, table, "speed", dynf32_value(100.f));
    self->acceleration = CONFIG_TRY_GET(DynamicVec2)(config, table, "acceleration", dynamic_vec2_value(vec2_zero()));
    self->scale = CONFIG_TRY_GET(DynamicVec2)(config, table, "scale", dynamic_vec2_value(vec2_one()));
    self->offset = CONFIG_TRY_GET(DynamicVec2)(config, table, "offset", dynamic_vec2_value(vec2_zero()));
    self->color = CONFIG_TRY_GET(DynamicColor)(config, table, "color", dynamic_color_value(color_make_rgb(255, 255, 255)));
    self->spawnArea = CONFIG_TRY_GET(Vec2)(config, table, "spawn_area", vec2_zero());
    self->worldSpace = CONFIG_TRY_GET(bool)(config, table, "world_space", false);
}

void particle_emitter_config_copy(const ParticleEmitterConfig* source, ParticleEmitterConfig* dest) {
    memcpy(dest, source, sizeof(ParticleEmitterConfig));

    dynf32_copy(&source->startingRotation, &dest->startingRotation);
    dynf32_copy(&source->rotationSpeed, &dest->rotationSpeed);
    dynf32_copy(&source->speed, &dest->speed);
    dynamic_vec2_copy(&source->acceleration, &dest->acceleration);
    dynamic_vec2_copy(&source->scale, &dest->scale);
    dynamic_vec2_copy(&source->offset, &dest->offset);
    dynamic_color_copy(&source->color, &dest->color);
}

void level_config_deserialize(TypeConfig* super, Config* config, char* table) {
    LevelConfig* self = (LevelConfig*)super;

    self->asteroidCount = (u32)CONFIG_GET(int)(config, table, "asteroids");
    self->metalAsteroidCount = (u32)CONFIG_TRY_GET(int)(config, table, "metal_asteroids", 0);
    self->maxSmallUfos = (u32)CONFIG_TRY_GET(int)(config, table, "max_small_ufo", 0);
    self->maxLargeUfos = (u32)CONFIG_TRY_GET(int)(config, table, "max_large_ufo", 0);
}

void level_manager_config_deserialize(TypeConfig* super, Config* config, char* table) {
    LevelManagerConfig* self = (LevelManagerConfig*)super;
    
    self->levelCount = (u32)config_get_array_count(config, table, "levels");
    self->levels = CALLOC(self->levelCount, LevelConfig*);

    for (u32 i = 0; i < self->levelCount; ++i) {
        LevelConfig* level = CONFIG_GET_AT(LevelConfig)(config, table, "levels", i);
        ASSERT(level, "");
        self->levels[i] = level;
    }

    self->startingLives = CONFIG_GET(i32)(config, table, "lives");
}

void level_manager_config_cleanup(TypeConfig* super) {
    LevelManagerConfig* self = (LevelManagerConfig*)super;

    if (self->levels) {
        free(self->levels);
    }
}