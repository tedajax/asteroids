#ifndef RUNNER_COLOR_H
#define RUNNER_COLOR_H

#include "types.h"

typedef struct color_t {
    u8 r, g, b, a;
} Color;

Color color_make_rgb(u8 r, u8 g, u8 b);
Color color_make_rgba(u8 r, u8 g, u8 b, u8 a);
Color color_make_rgbaf(f32 r, f32 g, f32 b, f32 a);
Color color_make_packed(u32 c);

void color_init_rgb(Color* self, u8 r, u8 g, u8 b);
void color_init_rgba(Color* self, u8 r, u8 g, u8 b, u8 a);
void color_init_rgbaf(Color* self, f32 r, f32 g, f32 b, f32 a);
void color_init_packed(Color* self, u32 c);

void color_lerp(Color* min, Color* max, f32 t, Color* dest);
void color_lerpf(Color* min, Color* max, f32 t, Color* dest);

#endif