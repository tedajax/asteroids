#ifndef RUNNER_COLLIDER_H
#define RUNNER_COLLIDER_H

#include "core.h"

#define COLLIDER_MAX_COLLISIONS 32

typedef enum collider_shape_e {
    COLLIDER_SHAPE_INVALID,
    COLLIDER_SHAPE_CIRCLE,
    COLLIDER_SHAPE_RECTANGLE,
    COLLIDER_SHAPE_LAST,
} ColliderShape;

typedef enum collider_layer_e {
    COLLIDER_LAYER_PLAYER,
    COLLIDER_LAYER_ENEMY,
    COLLIDER_LAYER_ENVIRONMENT,
    COLLIDER_LAYER_PLAYER_PROJECTILE,
    COLLIDER_LAYER_ENEMY_PROJECTILE,
    COLLIDER_LAYER_LAST,
} ColliderLayer;

typedef struct collider_t {
    i32 colliderId;
    ColliderLayer layer;
    ColliderShape shape;
    Circle circle;
    Rect rectangle;
    Vec2* anchor;
    i32 inContact[COLLIDER_MAX_COLLISIONS];
    u32 inContactCount;
} Collider;

void collider_init(Collider* self, ColliderLayer layer, Vec2* anchor);
void collider_init_circle(Collider* self, ColliderLayer layer, Vec2 offset, f32 radius, Vec2* anchor);
void collider_init_rectangle(Collider* self, ColliderLayer layer, Vec2 offset, f32 width, f32 height, Vec2* anchor);
void collider_copy(const Collider* source, Collider* dest);
bool collider_is_colliding(Collider* c1, Collider* c2);
bool collider_in_contact(Collider* self, Collider* other);
void collider_set_in_contact(Collider* c1, Collider* c2, bool inContact);
void collider_anchored_circle(Collider* self, Circle* dest);
void collider_anchored_rectangle(Collider* self, Rect* dest);

#endif