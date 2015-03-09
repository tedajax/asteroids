#ifndef ASTEROIDS_SPRITE_SYSTEM_H
#define ASTEROIDS_SPRITE_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "spritecomponent.h"

typedef struct sprite_system_t {
    AspectSystem super;
} SpriteSystem;

void sprite_system_init(SpriteSystem* self, EntityManager* entityManager);
void sprite_system_update(SpriteSystem* self);
void sprite_system_render(SpriteSystem* self);

void sprite_system_on_damage(AspectSystem* system, Entity entity, Message message);

#endif