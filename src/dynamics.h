#ifndef RUNNER_DYNAMICS_H
#define RUNNER_DYNAMICS_H

#include "types.h"
#include "algebra.h"
#include "dynf32.h"
#include "color.h"

typedef struct dynamic_vec2_t {
    Vec2 min;
    Vec2 max;
    dynf32 time;
} DynamicVec2;

typedef struct dynamic_color_t {
    Color min;
    Color max;
    dynf32 time;
} DynamicColor;

void dynamic_vec2_init(DynamicVec2* self, Vec2 min, Vec2 max, dynf32 time);
DynamicVec2 dynamic_vec2_value(Vec2 value);
void dynamic_vec2_release(DynamicVec2* self);
void dynamic_vec2_start(DynamicVec2* self, TweenManager* tweenManager);
void dynamic_vec2_copy(const DynamicVec2* source, DynamicVec2* dest);
Vec2 dynamic_vec2_get(DynamicVec2* self);

void dynamic_color_init(DynamicColor* self, Color min, Color max, dynf32 time);
DynamicColor dynamic_color_value(Color value);
void dynamic_color_release(DynamicColor* self);
void dynamic_color_start(DynamicColor* self, TweenManager* tweenManager);
void dynamic_color_copy(const DynamicColor* source, DynamicColor* dest);
Color dynamic_color_get(DynamicColor* self);

#endif