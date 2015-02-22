#ifndef RUNNER_DYNAMICS_H
#define RUNNER_DYNAMICS_H

#include "types.h"
#include "algebra.h"
#include "dynf32.h"
#include "color.h"

typedef struct dynamic_vec2_t {
    DynFloat2 min;
    DynFloat2 max;
    dynf32 time;
} DynamicVec2;

typedef struct dynamic_color_t {
    DynFloat4 min;
    DynFloat4 max;
    dynf32 time;
} DynamicColor;

void dynamic_vec2_init(DynamicVec2* self, DynFloat2 min, DynFloat2 max, dynf32 time);
DynamicVec2 dynamic_vec2_value(Vec2 value);
DynamicVec2 dynamic_vec2_dynfloat2(dynf32 x, dynf32 y);
void dynamic_vec2_release(DynamicVec2* self);
void dynamic_vec2_start(DynamicVec2* self, TweenManager* tweenManager);
void dynamic_vec2_copy(const DynamicVec2* source, DynamicVec2* dest);
Vec2 dynamic_vec2_get(DynamicVec2* self);
void dynamic_vec2_play(DynamicVec2* self);
void dynamic_vec2_pause(DynamicVec2* self);

void dynamic_color_init(DynamicColor* self, DynFloat4 min, DynFloat4 max, dynf32 time);
DynamicColor dynamic_color_value(Color value);
DynamicColor dynamic_color_dynfloat4(dynf32 x, dynf32 y, dynf32 z, dynf32 w);
void dynamic_color_release(DynamicColor* self);
void dynamic_color_start(DynamicColor* self, TweenManager* tweenManager);
void dynamic_color_copy(const DynamicColor* source, DynamicColor* dest);
Color dynamic_color_get(DynamicColor* self);
void dynamic_color_play(DynamicColor* self);
void dynamic_color_pause(DynamicColor* self);

#endif