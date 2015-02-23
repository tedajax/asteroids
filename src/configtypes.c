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
    self->lifetime = config_get_dynf32(config, table, "lifetime");
    self->speed = config_get_dynf32(config, table, "speed");
    self->angle = config_get_dynf32(config, table, "angle");
    self->fireDelay = config_get_dynf32(config, table, "fire_delay");
    self->startAngle = config_get_dynf32(config, table, "start_angle");
    self->damage = config_get_dynf32(config, table, "damage");
    
    self->textureName = config_get_string(config, table, "texture_name");

    dynf32_start(&self->burstShotDelay, &globals.tweens);
    dynf32_start(&self->spread, &globals.tweens);
    dynf32_start(&self->lifetime, &globals.tweens);
    dynf32_start(&self->speed, &globals.tweens);
    dynf32_start(&self->angle, &globals.tweens);
    dynf32_start(&self->fireDelay, &globals.tweens);
    dynf32_start(&self->startAngle, &globals.tweens);
    dynf32_start(&self->damage, &globals.tweens);

    self->colliderConfig = config_get_ColliderConfig(config, table, "collider");
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

void level_config_deserialize(TypeConfig* super, Config* config, char* table) {
    LevelConfig* self = (LevelConfig*)super;

    self->remove_this_later = 0;
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