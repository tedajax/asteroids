#include "range.h"
#include "config.h"

void range_init(Range* self, f32 min, f32 max) {
    self->min = min;
    self->max = max;
}

void random_range_init(RandomRange* self, f32 min, f32 max) {
    self->min = min;
    self->max = max;
}

f32 random_range_get(RandomRange* self) {
    return randf_range(self->min, self->max);
}