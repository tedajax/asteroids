#ifndef RUNNER_PARTICLES_H
#define RUNNER_PARTICLES_H

#include "core.h"
#include "config.h"
#include "atlas.h"

#include <SDL2/SDL.h>

typedef struct particle_t {
    Vec2 position;
    Vec2 direction;
    f32 speed;
    f32 lifetime;
    f32 alpha;
} Particle;

typedef struct particle_emitter_t {
    ParticleEmitterConfig* config;
    Particle* particles;
    Atlas* atlas;
    f32 emitTimer;
} ParticleEmitter;

// TODO: some sort of ParticleConfig struct
void particle_init(Particle* self, Vec2 position, Vec2 direction, f32 speed, f32 lifetime);
bool particle_dead(Particle* self);

// TODO: ParticleEmitterConfig struct
void emitter_init(ParticleEmitter* self, ParticleEmitterConfig* config);
void emitter_free(ParticleEmitter* self);

void emitter_update(ParticleEmitter* self);
i32 emitter_get_next_available(ParticleEmitter* self);

void emitter_render(ParticleEmitter* self, Vec2* position);


#endif