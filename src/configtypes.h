#ifndef RUNNER_CONFIG_TYPES_H
#define RUNNER_CONFIG_TYPES_H

#include "types.h"
#include "algebra.h"
#include "dynf32.h"
#include "dynamics.h"
#include "range.h"

typedef enum type_config_type_e {
    TYPE_CONFIG_COLLIDER = 0,
    TYPE_CONFIG_BULLET_SOURCE,
    TYPE_CONFIG_TWEEN,
    TYPE_CONFIG_PARTICLE_EMITTER,
    TYPE_CONFIG_LAST
} TypeConfigType;

typedef struct type_config_t {
    TypeConfigType type;
    char* tableName;
} TypeConfig;

typedef struct config_t Config;
typedef void(*deserialize_type_f)(TypeConfig*, Config*, char*);

deserialize_type_f deserializeFunctions[TYPE_CONFIG_LAST];

#define REGISTER_DESERIALIZE_FUNCTION(type, func) \
    deserializeFunctions[type] = func;

#define DESERIALIZE(type, typeconfig, config, table) \
    deserializeFunctions[type](typeconfig, config, table);

typedef enum collider_layer_e ColliderLayer;
typedef enum bounding_volume_type_e BoundingVolumeType;

typedef struct collider_config_t {
    TypeConfig super;
    i32 layer;
    Vec2 offset;
    i32 type;
    f32 width;
    f32 height;
    f32 radius;
    f32 orientation;
} ColliderConfig;

void collider_config_deserialize(TypeConfig* super, Config* config, char* table);
ColliderLayer collider_layer_parse(char* str);

typedef struct bullet_source_config_t {
    TypeConfig super;
    Vec2 offset;		// offset from parent
    i32 count;          // how many bullets to fire from this source
    i32 burstShotCount;     // number of shots fired in a burst (defaults to 1)
    i32 burstCount;         // number of bursts fired before requiring a release of fire button.
    dynf32 burstShotDelay;     // amount of time between shots in burst (defaults to 0)
    dynf32 spread;         // the spread angle of this source
    dynf32 lifetime;       // how long the bullet lives before they are destroyed
    dynf32 speed;          // how fast the bullet moves
    dynf32 angle;          // individual bullet angle adjustment (should be 0 unless it's dynamic)
    dynf32 fireDelay;      // Time between shots
    dynf32 startAngle;     // Direction to fire
    dynf32 damage;         // Damage done by bullet
    char* textureName;  // bullet texture
    ColliderConfig* colliderConfig;
} BulletSourceConfig;

void bullet_source_config_deserialize(TypeConfig* super, Config* config, char* table);
void bullet_source_config_copy(const BulletSourceConfig* source, BulletSourceConfig* dest);

typedef f32(*tween_func)(f32, f32, f32, f32);

typedef struct tween_config_t {
    TypeConfig super;
    RandomRange start;
    RandomRange end;
    f32 duration;
    f32 timescale;
    u32 loops;
    tween_func function;
} TweenConfig;

void tween_config_deserialize(TypeConfig* super, Config* config, char* table);

typedef struct level_config_t {
    u8 remove_this_later;
} LevelConfig;

void level_config_deserialize(TypeConfig* super, Config* config, char* table);

typedef struct particle_emitter_config_t {
    TypeConfig super;
    f32 lifetime;
    u32 maxParticles;
    u32 particlesPerEmission;
    f32 particleLifetime;
    f32 emissionInterval;
    f32 emissionBaseAngle;
    f32 emissionArcLength;
    dynf32 startingRotation;
    dynf32 rotationSpeed;
    dynf32 speed;
    DynamicVec2 acceleration;
    DynamicVec2 scale;
    DynamicVec2 offset;
    DynamicColor color;
    Vec2 spawnArea;
    bool worldSpace;
    char* atlasName;
    char* spriteName;
} ParticleEmitterConfig;

void particle_emitter_config_deserialize(TypeConfig* super, Config* config, char* table);

#endif