#include "particles.h"
#include "transformcomponent.h"

int particle_lifetime_compare(Particle* p1, Particle* p2) {
    if (p1->lifetime < p2->lifetime) {
        return -1;
    } else if (p1->lifetime > p2->lifetime) {
        return 1;
    } else {
        return 0;
    }
}

void emitter_sort(Particle** sortedList, u32 count, particle_compare_f compareFunc);

void particle_init(Particle* self, ParticleConfig* config) {
    vec2_copy_to(&config->position, &self->position);
    vec2_copy_to(&config->direction, &self->direction);
    dynamic_vec2_copy(config->acceleration, &self->acceleration);
    dynamic_vec2_start(&self->acceleration, &globals.tweens);
    dynamic_vec2_copy(config->scale, &self->scale);
    dynamic_vec2_start(&self->scale, &globals.tweens);
    dynamic_color_copy(config->color, &self->color);
    dynamic_color_start(&self->color, &globals.tweens);
    dynf32_copy(config->rotationSpeed, &self->rotationSpeed);
    dynf32_start(&self->rotationSpeed, &globals.tweens);
    dynf32_copy(config->speed, &self->speed);
    dynf32_start(&self->speed, &globals.tweens);
    self->rotation = config->rotation;
    self->lifetime = config->lifetime;
    self->velocity = vec2_zero();
}

void particle_cleanup(Particle* self) {
    dynamic_vec2_release(&self->acceleration);
    dynamic_vec2_release(&self->scale);
    dynamic_color_release(&self->color);
    dynf32_release(&self->rotationSpeed);
    dynf32_release(&self->speed);
}

bool particle_dead(Particle* self) {
    return self->lifetime <= 0.f;
}

void particle_play(Particle* self) {
    dynf32_play(&self->rotationSpeed);
    dynf32_play(&self->speed);
    dynamic_vec2_play(&self->acceleration);
    dynamic_vec2_play(&self->scale);
    dynamic_color_play(&self->color);
}

void particle_pause(Particle* self) {
    dynf32_pause(&self->rotationSpeed);
    dynf32_pause(&self->speed);
    dynamic_vec2_pause(&self->acceleration);
    dynamic_vec2_pause(&self->scale);
    dynamic_color_pause(&self->color);
}

void emitter_init(ParticleEmitter* self, ParticleEmitterConfig* config) {
    self->config = config;
    self->maxParticles = self->config->maxParticles;
    self->particles = CALLOC(self->config->maxParticles, Particle);
    self->sortedParticles = CALLOC(self->config->maxParticles, Particle*);
    self->activeParticles = 0;

    for (u32 i = 0; i < self->maxParticles; ++i) {
        self->sortedParticles[i] = &self->particles[i];
    }

    dynf32_copy(&config->startingRotation, &self->startingRotation);
    dynf32_start(&self->startingRotation, &globals.tweens);

    dynamic_vec2_copy(&config->offset, &self->offset);
    dynamic_vec2_start(&self->offset, &globals.tweens);

    self->emitTimer = self->config->emissionInterval;

    self->emitterVelocity = vec2_zero();

    self->atlas = atlas_get(config->atlasName);
    self->lifetime = config->lifetime;
    self->isPlaying = true;
    self->spawnNewParticles = true;
}

void emitter_free(ParticleEmitter* self) {
    free(self->particles);
    free(self->sortedParticles);
    dynf32_release(&self->startingRotation);
    dynamic_vec2_release(&self->offset);
}

void emitter_play(ParticleEmitter* self) {
    self->isPlaying = true;
    for (u32 i = 0; i < self->maxParticles; ++i) {
        if (!particle_dead(&self->particles[i])) {
            particle_play(&self->particles[i]);
        }
    }
    if (self->lifetime <= 0.f) {
        self->lifetime = self->config->lifetime;
        self->spawnNewParticles = true;
    }
}

void emitter_pause(ParticleEmitter* self) {
    for (u32 i = 0; i < self->maxParticles; ++i) {
        if (!particle_dead(&self->particles[i])) {
            particle_pause(&self->particles[i]);
        }
    }
    self->isPlaying = false;
}

void emitter_stop(ParticleEmitter* self) {
    self->lifetime = 0.f;
}

void emitter_update(ParticleEmitter* self, TransformComponent* anchor) {
    if (!self->isPlaying) {
        return;
    }
    
    // Update storage amounts if config changes max particles
    if (self->maxParticles < self->config->maxParticles) {
        Particle* newParticleList = CALLOC(self->config->maxParticles, Particle);
        memcpy(newParticleList, self->particles, self->maxParticles * sizeof(Particle));
        free(self->particles);
        self->particles = newParticleList;

        Particle** newSortedParticleList = CALLOC(self->config->maxParticles, Particle*);
        memcpy(newSortedParticleList, self->sortedParticles, self->maxParticles * sizeof(Particle*));
        free(self->sortedParticles);
        self->sortedParticles = newSortedParticleList;

        for (u32 i = 0; i < self->config->maxParticles; ++i) {
            self->sortedParticles[i] = &self->particles[i];
        }

        self->maxParticles = self->config->maxParticles;
    }

    // Update particles positions based on their properties
    for (u32 i = 0; i < self->maxParticles; ++i) {
        Particle* particle = &self->particles[i];

        if (particle_dead(particle)) {
            continue;
        }

        Vec2 acceleration = dynamic_vec2_get(&particle->acceleration);
        vec2_scale(&acceleration, globals.time.delta, &acceleration);
        vec2_add(&acceleration, &particle->velocity, &particle->velocity);

        Vec2 direction = particle->direction;
        if (!self->config->worldSpace) {
            //vec2_transform(&direction, anchor->rotation, &direction);
        }

        Vec2 velocity;
        vec2_scale(&direction, dynf32_get(&particle->speed), &velocity);
        vec2_add(&velocity, &particle->velocity, &velocity);
        vec2_add(&velocity, &self->emitterVelocity, &velocity);

        vec2_scale(&velocity, globals.time.delta, &velocity);

        vec2_add(&particle->position, &velocity, &particle->position);

        particle->rotation += dynf32_get(&particle->rotationSpeed) * globals.time.delta;

        particle->lifetime -= globals.time.delta;

        if (particle_dead(particle)) {
            particle_cleanup(particle);
            --self->activeParticles;
        }
    }

    if (self->lifetime > 0.f) {
        self->lifetime -= globals.time.delta;
        if (self->lifetime <= 0.f) {
            self->spawnNewParticles = false;
        }
    }

    // If we're not being instructed to spawn new particles we exit
    if (!self->spawnNewParticles) {
        emitter_sort(self->sortedParticles, self->maxParticles, particle_lifetime_compare);
        return;
    }

    // Update emit timer and if it's reached the emit interval spawn a wave of particles.
    self->emitTimer += globals.time.delta;

    if (self->emitTimer < self->config->emissionInterval) {
        emitter_sort(self->sortedParticles, self->maxParticles, particle_lifetime_compare);
        return;
    }

    // Spawn particles
    for (u32 i = 0; i < self->config->particlesPerEmission; ++i) {
        i32 index = emitter_get_next_available(self);
        if (index < 0) {
            break;
        }

        self->newestParticle = index;

        Particle* particle = &self->particles[index];

        f32 angle = self->config->emissionBaseAngle;
        f32 arc = self->config->emissionArcLength / 2.f;
        angle += randf_range(-arc, arc);
        if (self->config->worldSpace) {
            angle += anchor->rotation;
        }

        Vec2 direction;
        vec2_set_angle(&direction, angle, 1.f);

        f32 w = self->config->spawnArea.x / 2.f;
        f32 h = self->config->spawnArea.y / 2.f;
        Vec2 spawnPos = vec2_rand_range(-w, -h, w, h);
        vec2_transform(&spawnPos, anchor->rotation, &spawnPos);

        Vec2 offset = dynamic_vec2_get(&self->offset);
        vec2_transform(&offset, anchor->rotation, &offset);

        if (self->config->worldSpace) {
            vec2_add(&spawnPos, &offset, &spawnPos);
            vec2_add(&spawnPos, &anchor->position, &spawnPos);
        }

        ParticleConfig particleCfg = {
            spawnPos,
            direction,
            &self->config->acceleration,
            &self->config->scale,
            &self->config->color,
            &self->config->rotationSpeed,
            &self->config->speed,
            dynf32_get(&self->config->startingRotation),
            self->config->particleLifetime,
        };

        particle_init(particle, &particleCfg);
        ++self->activeParticles;
    }

    self->emitTimer = 0;

    emitter_sort(self->sortedParticles, self->maxParticles, particle_lifetime_compare);
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

    for (u32 i = 0; i < self->maxParticles; ++i) {
        Particle* particle = self->sortedParticles[i];
        
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

        Vec2 position = particle->position;
        if (!self->config->worldSpace) {
            vec2_transform(&position, anchor->rotation, &position);
            Vec2 offset = dynamic_vec2_get(&self->offset);
            vec2_transform(&offset, anchor->rotation, &offset);
            vec2_add(&position, &offset, &position);
        }

        SDL_Rect dest;
        dest.x = (int)position.x - (int)halfWidth;
        dest.y = (int)position.y - (int)halfHeight;
        dest.w = (int)width;
        dest.h = (int)height;

        SDL_Rect src;
        src.x = (int)frame->frame.position.x;
        src.y = (int)frame->frame.position.y;
        src.w = (int)frame->frame.width;
        src.h = (int)frame->frame.height;

        f32 rotation = particle->rotation;

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

void emitter_sort(Particle** sortedList, u32 count, particle_compare_f compareFunc) {
    profiler_tick("emitter_sort");
    for (u32 i = 1; i < count; ++i) {
        u32 j = i;
        Particle* p1 = sortedList[j - 1];
        Particle* p2 = sortedList[j];
        while (j > 0 && compareFunc(p1, p2) < 0) {
            Particle* temp = sortedList[j - 1];
            sortedList[j - 1] = sortedList[j];
            sortedList[j] = temp;
            --j;
        }
    }

    profiler_tock("emitter_sort");
}