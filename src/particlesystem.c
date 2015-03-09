#include "particlesystem.h"

void particle_system_init(ParticleSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_PARTICLE, MAX_ENTITIES);
}

void particle_system_update(ParticleSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);

        ParticleComponent* particle =
            (ParticleComponent*)GET_SYSTEM_COMPONENT(i);

        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        REQUIRED_COMPONENTS(transform, particle);

        bool readyToRemove = true;

        for (u32 j = 0; j < particle->emitterCount; ++j) {
            emitter_update(&particle->emitters[j], transform);
            if (!emitter_done(&particle->emitters[j])) {
                readyToRemove = false;
            }
        }

        if (particle->removeOnDone && readyToRemove) {
            entities_remove_entity(self->super.entityManager, entity);
        }
    }
}

void particle_system_render(ParticleSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);

        ParticleComponent* particle =
            (ParticleComponent*)GET_SYSTEM_COMPONENT(i);

        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        REQUIRED_COMPONENTS(transform, particle);

        for (u32 j = 0; j < particle->emitterCount; ++j) {
            emitter_render(&particle->emitters[j], transform);
        }
    }
}