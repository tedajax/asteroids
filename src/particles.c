#include "particles.h"
#include "transformcomponent.h"

void particle_init(Particle* self, ParticleConfig* config) {
    vec2_copy_to(&config->position, &self->position);
    vec2_copy_to(&config->direction, &self->direction);
    dynamic_vec2_copy(config->scale, &self->scale);
    dynamic_vec2_start(&self->scale, &globals.tweens);
    dynamic_color_copy(config->color, &self->color);
    dynamic_color_start(&self->color, &globals.tweens);
    self->speed = config->speed;
    self->lifetime = config->lifetime;
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

        if (particle_dead(particle)) {
            dynamic_vec2_release(&particle->scale);
            dynamic_color_release(&particle->color);
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

        ParticleConfig particleCfg = {
            spawnPos,
            direction,
            &self->config->scale,
            &self->config->color,
            100.f,
            1.f
        };

        particle_init(particle, &particleCfg);
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

    for (i32 i = (i32)self->config->maxParticles - 1; i >= 0; --i) {
        Particle* particle = &self->particles[i];
        
        if (particle_dead(particle)) {
            continue;
        }

        Color color = dynamic_color_get(&particle->color);
        SDL_SetTextureColorMod(self->atlas->texture, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(self->atlas->texture, color.a);

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

    SDL_SetTextureColorMod(self->atlas->texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(self->atlas->texture, 255);
}