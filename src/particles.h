#ifndef RUNNER_PARTICLES_H
#define RUNNER_PARTICLES_H

#include "core.h"
#include "config.h"
#include "atlas.h"
#include "dynamics.h"

#include <SDL2/SDL.h>

typedef struct transform_component_t TransformComponent;

typedef struct particle_t {
    Vec2 position;
    Vec2 velocity;
    Vec2 direction;
    DynamicVec2 acceleration;
    DynamicVec2 scale;
    DynamicColor color;
    dynf32 rotationSpeed;
    dynf32 speed;
    f32 rotation;
    f32 lifetime;
} Particle;

typedef struct particle_config_t {
    Vec2 position;
    Vec2 direction;
    DynamicVec2* acceleration;
    DynamicVec2* scale;
    DynamicColor* color;
    dynf32* rotationSpeed;
    dynf32* speed;
    f32 rotation;
    f32 lifetime;
} ParticleConfig;

typedef struct particle_emitter_t {
    ParticleEmitterConfig* config;
    Particle* particles;
    Particle** sortedParticles;
    Atlas* atlas;
    dynf32 startingRotation;
    u32 maxParticles;
    u32 activeParticles;
    i32 newestParticle;
    f32 lifetime;
    f32 emitTimer;
    bool isPlaying;
    bool spawnNewParticles;
} ParticleEmitter;

typedef int(*particle_compare_f)(Particle*, Particle*);

// TODO: some sort of ParticleConfig struct
void particle_init(Particle* self, ParticleConfig* config);
void particle_cleanup(Particle* self);
bool particle_dead(Particle* self);
void particle_play(Particle* self);
void particle_pause(Particle* self);

// TODO: ParticleEmitterConfig struct
void emitter_init(ParticleEmitter* self, ParticleEmitterConfig* config);
void emitter_free(ParticleEmitter* self);

void emitter_play(ParticleEmitter* self);
void emitter_pause(ParticleEmitter* self);
void emitter_stop(ParticleEmitter* self);

void emitter_update(ParticleEmitter* self, TransformComponent* anchor);
i32 emitter_get_next_available(ParticleEmitter* self);

void emitter_render(ParticleEmitter* self, TransformComponent* anchor);


#endif