#include "camera.h"
#include "globals.h"

void camera_init(Camera* self, Vec2* target) {
    self->position = vec2_zero();

    self->target = target;

    rect_set(&self->cameraView, &self->position,
        (f32)globals.world.width,
        (f32)globals.world.height);
}

void camera_update(Camera* self) {
    self->position.x = globals.levelPosition;

    rect_set(&self->worldView, &self->position,
        (f32)globals.world.width,
        (f32)globals.world.height);
}