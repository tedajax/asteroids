#ifndef ASTEROIDS_RANGE_H
#define ASTEROIDS_RANGE_H

#include "types.h"
#include "algebra.h"

typedef struct range_t {
    f32 min;
    f32 max;
} Range;

void range_init(Range* self, f32 min, f32 max);

inline f32 range_rand(Range* self) {
    return randf_range(self->min, self->max);
}

inline f32 range_mid(Range* self) {
    return (self->max + self->min) / 2.f;
}

typedef struct random_range_t {
    f32 min;
    f32 max;
} RandomRange;

void random_range_init(RandomRange* self, f32 min, f32 max);
f32 random_range_get(RandomRange* self);

#endif