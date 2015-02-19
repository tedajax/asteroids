#include "particles.h"

void particle_init(Particle* self, Vec2 position, Vec2 direction, f32 speed, f32 lifetime) {
    vec2_copy_to(&position, &self->position);
    vec2_copy_to(&direction, &self->direction);
    self->speed = speed;
    self->lifetime = lifetime;
    self->alpha = 255;
}

bool particle_dead(Particle* self) {
    return self->lifetime <= 0.f;
}

void emitter_init(ParticleEmitter* self, ParticleEmitterConfig* config) {
    self->config = config;
    self->particles = CALLOC(self->config->maxParticles, Particle);

    self->emitTimer = 0.f;

    self->atlas = atlas_get(config->atlasName);
}

void emitter_free(ParticleEmitter* self) {
    free(self->particles);
}

void emitter_update(ParticleEmitter* self) {
    self->emitTimer += globals.time.delta;

    for (u32 i = 0; i < self->config->maxParticles; ++i) {
        Particle* particle = &self->particles[i];

        if (particle_dead(particle)) {
            continue;
        }

        f32 scaledSpeed = particle->speed * globals.time.delta;
        Vec2 velocity;
        vec2_scale(&particle->direction, scaledSpeed, &velocity);
        vec2_add(&particle->position, &velocity, &particle->position);

        particle->lifetime -= globals.time.delta;
        particle->alpha = particle->lifetime / 1.f;
    }

    if (self->emitTimer < self->config->emissionInterval) {
        return;
    }

    for (u32 i = 0; i < self->config->particlesPerEmission; ++i) {
        i32 index = emitter_get_next_available(self);
        ASSERT(index >= 0, "Reach maximum number of particles for emitter.  Change settings or make this work gracefully.");

        Particle* particle = &self->particles[index];

        particle_init(particle, vec2_zero(), vec2_rand_direction(), 100.f, 1.f);
    }

    self->emitTimer = 0;
}

// TODO: this is going to be pretty slow so we'll want to do some sort of sorting or something to allow for some faster lookups
i32 emitter_get_next_available(ParticleEmitter* self) {
    for (u32 i = 0; i < self->config->maxParticles; ++i) {
        if (particle_dead(&self->particles[i])) {
            return i;
        }
    }

    return -1;
}

void emitter_render(ParticleEmitter* self, Vec2* position) {
    SpriteFrame* frame = atlas_get_frame(self->atlas, self->config->spriteName);

    for (u32 i = 0; i < self->config->maxParticles; ++i) {
        Particle* particle = &self->particles[i];
        
        if (particle_dead(particle)) {
            continue;
        }

        SDL_SetTextureAlphaMod(self->atlas->texture, (u8)(particle->alpha * 255.f));

        SDL_Rect dest;
        dest.x = (int)particle->position.x + (int)position->x;
        dest.y = (int)particle->position.y + (int)position->y;
        dest.w = (int)frame->frame.width;
        dest.h = (int)frame->frame.height;

        SDL_Rect src;
        src.x = (int)frame->frame.position.x;
        src.y = (int)frame->frame.position.y;
        src.w = (int)frame->frame.width;
        src.h = (int)frame->frame.height;

        SDL_RenderCopyEx(globals.renderer,
            self->atlas->texture,
            &src,
            &dest,
            0.f,
            NULL,
            SDL_FLIP_NONE);
    }

    SDL_SetTextureAlphaMod(self->atlas->texture, 255);
}