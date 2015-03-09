#ifndef ASTEROIDS_PARTICLE_COMPONENT_H
#define ASTEROIDS_PARTICLE_COMPONENT_H

#include "core.h"
#include "component.h"
#include "particles.h"

typedef struct particle_component_t {
    Component super;
    ParticleEmitter* emitters;
    u32 emitterCount;
} ParticleComponent;

ParticleComponent* particle_component_new(Entity entity, ParticleEmitterConfig** configs, u32 count);
COMPONENT_DEFINE(COMPONENT_PARTICLE);

#endif