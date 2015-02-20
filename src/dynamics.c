#include "dynamics.h"

void dynamic_vec2_init(DynamicVec2* self, Vec2 min, Vec2 max, dynf32 time) {
    self->min = min;
    self->max = max;
    self->time = time;
}

DynamicVec2 dynamic_vec2_value(Vec2 value) {
    DynamicVec2 result;

    result.min = value;
    result.max = value;
    result.time = dynf32_value(0.f);

    return result;
}

void dynamic_vec2_release(DynamicVec2* self) {
    dynf32_release(&self->time);
}

void dynamic_vec2_start(DynamicVec2* self, TweenManager* tweenManager) {
    dynf32_start(&self->time, tweenManager);
}

void dynamic_vec2_copy(const DynamicVec2* source, DynamicVec2* dest) {
    dest->min = source->min;
    dest->max = source->max;
    dynf32_copy(&source->time, &dest->time);
}

Vec2 dynamic_vec2_get(DynamicVec2* self) {
    Vec2 result;
    vec2_lerp(&self->min, &self->max, dynf32_get(&self->time), &result);
    return result;
}

void dynamic_color_init(DynamicColor* self, Color min, Color max, dynf32 time) {
    self->min = min;
    self->max = max;
    self->time = time;
}

DynamicColor dynamic_color_value(Color value) {
    DynamicColor result;

    result.min = value;
    result.max = value;
    result.time = dynf32_value(0.f);

    return result;
}

void dynamic_color_release(DynamicColor* self) {
    dynf32_release(&self->time);
}

void dynamic_color_start(DynamicColor* self, TweenManager* tweenManager) {
    dynf32_start(&self->time, tweenManager);
}

void dynamic_color_copy(const DynamicColor* source, DynamicColor* dest) {
    dynf32_copy(&source->time, &dest->time);
}

Color dynamic_color_get(DynamicColor* self) {
    Color result;
    color_lerp(&self->min, &self->max, dynf32_get(&self->time), &result);
}