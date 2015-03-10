#ifndef ASTEROIDS_PARTICLE_SYSTEM_H
#define ASTEROIDS_PARTICLE_SYSTEM_H

#include "aspectsystem.h"
#include "particlecomponent.h"
#include "transformcomponent.h"

typedef struct particle_system_t{
    AspectSystem super;
} ParticleSystem;

void particle_system_init(ParticleSystem* self, GameScene* scene);
void particle_system_update(ParticleSystem* self);
void particle_system_render(ParticleSystem* self);

#endif