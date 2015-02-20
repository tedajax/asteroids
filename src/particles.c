#include "particles.h"
#include "transformcomponent.h"

void particle_init(Particle* self, Vec2 position, Vec2 direction, DynamicVec2* scale, f32 speed, f32 lifetime) {
    vec2_copy_to(&position, &self->position);
    vec2_copy_to(&direction, &self->direction);
    dynamic_vec2_copy(scale, &self->scale);
    dynamic_vec2_start(&self->scale, &globals.tweens);
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

void emitter_update(ParticleEmitter* self, TransformComponent* anchor) {
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

        if (particle_dead(particle)) {
            dynamic_vec2_release(&particle->scale);
        }
    }

    if (self->emitTimer < self->config->emissionInterval) {
        return;
    }

    for (u32 i = 0; i < self->config->particlesPerEmission; ++i) {
        i32 index = emitter_get_next_available(self);
        ASSERT(index >= 0, "Reach maximum number of particles for emitter.  Change settings or make this work gracefully.");

        Particle* particle = &self->particles[index];

        f32 angle = self->config->emissionBaseAngle;
        f32 arc = self->config->emissionArcLength / 2.f;
        angle += randf_range(-arc, arc);
        angle += anchor->rotation;

        Vec2 direction;
        vec2_set_angle(&direction, angle, 1.f);

        f32 w = self->config->spawnArea.x / 2.f;
        f32 h = self->config->spawnArea.y / 2.f;
        Vec2 spawnPos = vec2_rand_range(-w, -h, w, h);
        vec2_tranform(&spawnPos, anchor->rotation, &spawnPos);

        if (self->config->worldSpace) {
            vec2_add(&spawnPos, &anchor->position, &spawnPos);
        }

        particle_init(particle, spawnPos, direction, &self->config->scale, 100.f, 1.f);
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

void emitter_render(ParticleEmitter* self, TransformComponent* anchor) {
    SpriteFrame* frame = atlas_get_frame(self->atlas, self->config->spriteName);

    for (u32 i = 0; i < self->config->maxParticles; ++i) {
        Particle* particle = &self->particles[i];
        
        if (particle_dead(particle)) {
            continue;
        }

        SDL_SetTextureAlphaMod(self->atlas->texture, (u8)(particle->alpha * 255.f));

        Vec2 scale = dynamic_vec2_get(&particle->scale);
        f32 width = frame->frame.width * scale.x;
        f32 height = frame->frame.height * scale.y;
        f32 halfWidth = width / 2.f;
        f32 halfHeight = height / 2.f;

        SDL_Rect dest;
        dest.x = (int)particle->position.x - (int)halfWidth;
        dest.y = (int)particle->position.y - (int)halfHeight;
        dest.w = (int)width;
        dest.h = (int)height;

        SDL_Rect src;
        src.x = (int)frame->frame.position.x;
        src.y = (int)frame->frame.position.y;
        src.w = (int)frame->frame.width;
        src.h = (int)frame->frame.height;

        f32 rotation = 0.f;

        if (!self->config->worldSpace) {
            dest.x += (int)anchor->position.x;
            dest.y += (int)anchor->position.y;
            rotation = anchor->rotation;
        }

        SDL_RenderCopyEx(globals.renderer,
            self->atlas->texture,
            &src,
            &dest,
            rotation,
            NULL,
            SDL_FLIP_NONE);
    }

    SDL_SetTextureAlphaMod(self->atlas->texture, 255);
}