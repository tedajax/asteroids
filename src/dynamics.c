#include "dynamics.h"

void dynamic_vec2_init(DynamicVec2* self, DynFloat2 min, DynFloat2 max, dynf32 time) {
    self->min = min;
    self->max = max;
    self->time = time;
}

DynamicVec2 dynamic_vec2_value(Vec2 value) {
    DynamicVec2 result;

    result.min = dynfloat2_create(dynf32_value(value.x), dynf32_value(value.y));
    result.max = dynfloat2_create(dynf32_value(value.x), dynf32_value(value.y));
    result.time = dynf32_value(0.f);

    return result;
}

DynamicVec2 dynamic_vec2_dynfloat2(dynf32 x, dynf32 y) {
    DynamicVec2 result;

    result.min = dynfloat2_create(x, y);
    result.max = dynfloat2_create(x, y);
    result.time = dynf32_value(0.f);

    return result;
}

void dynamic_vec2_release(DynamicVec2* self) {
    dynfloat2_release(&self->min);
    dynfloat2_release(&self->max);
    dynf32_release(&self->time);
}

void dynamic_vec2_start(DynamicVec2* self, TweenManager* tweenManager) {
    dynfloat2_start(&self->min, tweenManager);
    dynfloat2_start(&self->max, tweenManager);
    dynf32_start(&self->time, tweenManager);
}

void dynamic_vec2_copy(const DynamicVec2* source, DynamicVec2* dest) {
    dynfloat2_copy(&source->min, &dest->min);
    dynfloat2_copy(&source->max, &dest->max);
    dynf32_copy(&source->time, &dest->time);
}

Vec2 dynamic_vec2_get(DynamicVec2* self) {
    Vec2 result;
    Vec2 minVec = vec2_init(dynfloat2_get_x(&self->min), dynfloat2_get_y(&self->min));
    Vec2 maxVec = vec2_init(dynfloat2_get_x(&self->max), dynfloat2_get_y(&self->max));
    vec2_lerp(&minVec, &maxVec, dynf32_get(&self->time), &result);
    return result;
}

void dynamic_vec2_play(DynamicVec2* self) {
    dynfloat2_play(&self->min);
    dynfloat2_play(&self->max);
    dynf32_play(&self->time);
}

void dynamic_vec2_pause(DynamicVec2* self) {
    dynfloat2_pause(&self->min);
    dynfloat2_pause(&self->max);
    dynf32_pause(&self->time);
}


void dynamic_color_init(DynamicColor* self, DynFloat4 min, DynFloat4 max, dynf32 time) {
    self->min = min;
    self->max = max;
    self->time = time;
}

DynamicColor dynamic_color_value(Color value) {
    DynamicColor result;

    result.min = dynfloat4_create(dynf32_value(value.r), dynf32_value(value.g), dynf32_value(value.b), dynf32_value(value.a));
    result.max = dynfloat4_create(dynf32_value(value.r), dynf32_value(value.g), dynf32_value(value.b), dynf32_value(value.a));
    result.time = dynf32_value(0.f);

    return result;
}

DynamicColor dynamic_color_dynfloat4(dynf32 x, dynf32 y, dynf32 z, dynf32 w) {
    DynamicColor result;

    result.min = dynfloat4_create(x, y, z, w);
    result.max = dynfloat4_create(x, y, z, w);
    result.time = dynf32_value(0.f);

    return result;
}

void dynamic_color_release(DynamicColor* self) {
    dynfloat4_release(&self->min);
    dynfloat4_release(&self->max);
    dynf32_release(&self->time);
}

void dynamic_color_start(DynamicColor* self, TweenManager* tweenManager) {
    dynfloat4_start(&self->min, tweenManager);
    dynfloat4_start(&self->max, tweenManager);
    dynf32_start(&self->time, tweenManager);
}

void dynamic_color_copy(const DynamicColor* source, DynamicColor* dest) {
    dynfloat4_copy(&source->min, &dest->min);
    dynfloat4_copy(&source->max, &dest->max);
    dynf32_copy(&source->time, &dest->time);
}

Color dynamic_color_get(DynamicColor* self) {
    Color result;
    Color minColor = color_make_rgbaf(dynfloat4_get_x(&self->min), dynfloat4_get_y(&self->min), dynfloat4_get_z(&self->min), dynfloat4_get_w(&self->min));
    Color maxColor = color_make_rgbaf(dynfloat4_get_x(&self->max), dynfloat4_get_y(&self->max), dynfloat4_get_z(&self->max), dynfloat4_get_w(&self->max));
    color_lerpf(&minColor, &maxColor, dynf32_get(&self->time), &result);
    return result;
}

void dynamic_color_play(DynamicColor* self) {
    dynfloat4_play(&self->min);
    dynfloat4_play(&self->max);
    dynf32_play(&self->time);
}

void dynamic_color_pause(DynamicColor* self) {
    dynfloat4_pause(&self->min);
    dynfloat4_pause(&self->max);
    dynf32_pause(&self->time);
}