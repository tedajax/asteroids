#include "particlecomponent.h"

ParticleComponent* particle_component_new(Entity entity, ParticleEmitterConfig** configs, u32 count) {
    ParticleComponent* self = CALLOC(1, ParticleComponent);

    component_init((Component*)self, COMPONENT_PARTICLE, sizeof(ParticleComponent), entity);

    self->emitters = CALLOC(count, ParticleEmitter);
    self->emitterCount = count;

    for (u32 i = 0; i < self->emitterCount; ++i) {
        emitter_init(&self->emitters[i], configs[i]);
    }

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_PARTICLE) {
    u32 count = 1;
    if (config_is_array(config, table, "emitters")) {
        count = config_get_array_count(config, table, "emitters");
    }
    
    ParticleEmitterConfig** configs = CALLOC(count, ParticleEmitterConfig*);

    for (u32 i = 0; i < count; ++i) {
        ParticleEmitterConfig* emitterConfig = CONFIG_GET_AT(ParticleEmitterConfig)(config, table, "emitters", i);
        configs[i] = emitterConfig;
    }

    ParticleComponent* result = particle_component_new(0, configs, count);

    result->removeOnDone = CONFIG_TRY_GET(bool)(config, table, "remove_on_done", false);

    free(configs);
    return (Component*)result;
}

COMPONENT_FREE(COMPONENT_PARTICLE) {
    ParticleComponent* self = (ParticleComponent*)component;
    for (u32 i = 0; i < self->emitterCount; ++i) {
        emitter_free(&self->emitters[i]);
    }
    free(self->emitters);
}

COMPONENT_COPY(COMPONENT_PARTICLE) {
    ParticleComponent* particleSource = (ParticleComponent*)source;
    ParticleComponent* particleDest = (ParticleComponent*)dest;

    particleDest->emitters = CALLOC(particleSource->emitterCount, ParticleEmitter);
    particleDest->emitterCount = particleSource->emitterCount;

    for (u32 i = 0; i < particleDest->emitterCount; ++i) {
        emitter_init(&particleDest->emitters[i], particleSource->emitters[i].configSource);
    }
}