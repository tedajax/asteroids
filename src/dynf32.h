#ifndef ASTEROIDS_DYNF32_H
#define ASTEROIDS_DYNF32_H

#include "types.h"
#include "inline.h"
#include "ini.h"

// forward declarations
typedef struct tween_t Tween;
typedef struct tween_config_t TweenConfig;
typedef struct tween_manager_t TweenManager;

typedef enum dynamic_f32_type_e {
    DYN_F32_VALUE,
    DYN_F32_TWEEN,
} DynamicFloatType;

typedef struct dynamic_f32_t {
    f32 value;
    DynamicFloatType type;
    Tween* tween;
    TweenConfig* tweenConfig;
} dynf32;

void dynf32_zero(dynf32* self);
dynf32 dynf32_value(f32 value);
void dynf32_release(dynf32* self);
void dynf32_start(dynf32* self, TweenManager* tweenManager);
void dynf32_copy(const dynf32* source, dynf32* dest);
f32 dynf32_get(dynf32* self);
void dynf32_set(dynf32* self, f32 value);
void dynf32_play(dynf32* self);
void dynf32_pause(dynf32* self);

typedef struct dynamic_f32_2_t {
    dynf32 x, y;
} DynFloat2;

void dynfloat2_zero(DynFloat2* self);
DynFloat2 dynfloat2_create(dynf32 x, dynf32 y);
void dynfloat2_release(DynFloat2* self);
void dynfloat2_start(DynFloat2* self, TweenManager* tweenManager);
void dynfloat2_copy(const DynFloat2* source, DynFloat2* dest);
f32 dynfloat2_get_x(DynFloat2* self);
f32 dynfloat2_get_y(DynFloat2* self);
void dynfloat2_set_x(DynFloat2* self, f32 value);
void dynfloat2_set_y(DynFloat2* self, f32 value);
void dynfloat2_play(DynFloat2* self);
void dynfloat2_pause(DynFloat2* self);

typedef struct dynamic_f32_3_t {
    dynf32 x, y, z;
} DynFloat3;

void dynfloat3_zero(DynFloat3* self);
DynFloat3 dynfloat3_create(dynf32 x, dynf32 y, dynf32 z);
void dynfloat3_release(DynFloat3* self);
void dynfloat3_start(DynFloat3* self, TweenManager* tweenManager);
void dynfloat3_copy(const DynFloat3* source, DynFloat3* dest);
f32 dynfloat3_get_x(DynFloat3* self);
f32 dynfloat3_get_y(DynFloat3* self);
f32 dynfloat3_get_z(DynFloat3* self);
void dynfloat3_set_x(DynFloat3* self, f32 value);
void dynfloat3_set_y(DynFloat3* self, f32 value);
void dynfloat3_set_z(DynFloat3* self, f32 value);
void dynfloat3_play(DynFloat3* self);
void dynfloat3_pause(DynFloat3* self);

typedef struct dynamic_f32_4_t {
    dynf32 x, y, z, w;
} DynFloat4;

void dynfloat4_zero(DynFloat4* self);
DynFloat4 dynfloat4_create(dynf32 x, dynf32 y, dynf32 z, dynf32 w);
void dynfloat4_release(DynFloat4* self);
void dynfloat4_start(DynFloat4* self, TweenManager* tweenManager);
void dynfloat4_copy(const DynFloat4* source, DynFloat4* dest);
f32 dynfloat4_get_x(DynFloat4* self);
f32 dynfloat4_get_y(DynFloat4* self);
f32 dynfloat4_get_z(DynFloat4* self);
f32 dynfloat4_get_w(DynFloat4* self);
void dynfloat4_set_x(DynFloat4* self, f32 value);
void dynfloat4_set_y(DynFloat4* self, f32 value);
void dynfloat4_set_z(DynFloat4* self, f32 value);
void dynfloat4_set_w(DynFloat4* self, f32 value);
void dynfloat4_play(DynFloat4* self);
void dynfloat4_pause(DynFloat4* self);

#endif