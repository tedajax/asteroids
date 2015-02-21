#include "color.h"
#include "algebra.h"

Color color_make_rgb(u8 r, u8 g, u8 b) {
    return color_make_rgba(r, g, b, 255);
}

Color color_make_rgba(u8 r, u8 g, u8 b, u8 a) {
    Color result;
    result.r = r; result.g = g; result.b = b; result.a = a;
    return result;
}

Color color_make_rgbaf(f32 r, f32 g, f32 b, f32 a) {
    return color_make_rgba((u8)(r * 255.f), (u8)(g * 255.f), (u8)(b * 255.f), (u8)(a * 255.f));
}

Color color_make_packed(u32 c) {
    return color_make_rgba((c >> 24) & 0xFF, (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
}

void color_init_rgb(Color* self, u8 r, u8 g, u8 b) {
    color_init_rgba(self, r, g, b, 255);
}

void color_init_rgba(Color* self, u8 r, u8 g, u8 b, u8 a) {
    self->r = r; self->g = g; self->b = b; self->a = a;
}

void color_init_rgbaf(Color* self, f32 r, f32 g, f32 b, f32 a) {
    self->r = (u8)(r * 255.f);
    self->g = (u8)(g * 255.f);
    self->b = (u8)(b * 255.f);
    self->a = (u8)(a * 255.f);
}

void color_init_packed(Color* self, u32 c) {
    color_init_rgba(self, (c >> 24) & 0xFF, (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
}

void color_lerp(Color* min, Color* max, f32 t, Color* dest) {
    f32 rminf = (f32)min->r / 255.f;
    f32 gminf = (f32)min->g / 255.f;
    f32 bminf = (f32)min->b / 255.f;
    f32 aminf = (f32)min->a / 255.f;
    f32 rmaxf = (f32)max->r / 255.f;
    f32 gmaxf = (f32)max->g / 255.f;
    f32 bmaxf = (f32)max->b / 255.f;
    f32 amaxf = (f32)max->a / 255.f;

    f32 r = lerpf(rminf, rmaxf, t);
    f32 g = lerpf(gminf, gmaxf, t);
    f32 b = lerpf(bminf, bmaxf, t);
    f32 a = lerpf(aminf, amaxf, t);
    *dest = color_make_rgbaf(r, g, b, a);
}