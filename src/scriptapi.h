#ifndef ASTEROIDS_SCRIPT_API_H
#define ASTEROIDS_SCRIPT_API_H

#include "tdjxlua.h"
#include "types.h"
#include "algebra.h"
#include "entity.h"

// TODO: Everything in here should be autogenerated
#if 0
typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef float       f32;
typedef double      f64;

typedef struct vec_2_t {
    f32 x, y;
} Vec2;

typedef i32 Entity;

typedef enum component_type_e {
    COMPONENT_INVALID,

    COMPONENT_TRANSFORM,
    COMPONENT_MOVEMENT,

    COMPONENT_CONTROLLER,
    COMPONENT_BULLET_CONTROLLER,
    COMPONENT_GRAVITY,

    COMPONENT_HEALTH,
    COMPONENT_SPRITE,

    COMPONENT_BG_MANAGER,

    COMPONENT_ENEMY,

    COMPONENT_COLLIDER,

    COMPONENT_LUA,

    COMPONENT_LAST,
} ComponentType;

typedef struct component_t {
    ComponentType type;
    u64 size;
    Entity entity;
} Component;

typedef struct transform_component_t {
    Component super;
    Vec2 position;
    Vec2 scale;
    f32 rotation;
} TransformComponent;
#endif

LUA_EXPORT void quit();
void Sleep(int ms);
LUA_EXPORT int get_time_ms();

LUA_EXPORT void draw_rectangle(Vec2 center, f32 width, f32 height);
LUA_EXPORT void draw_rectangle_rgba(Vec2 center, f32 width, f32 height, u8 r, u8 g, u8 b, u8 a);
LUA_EXPORT void instantiate(const char* name);
LUA_EXPORT void instantiate_at(const char* name, Vec2 position, f32 rotation);
LUA_EXPORT void destroy(Entity entity);
LUA_EXPORT void get_transform(Entity entity);

#endif