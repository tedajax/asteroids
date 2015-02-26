#ifndef RUNNER_CAMERA_H
#define RUNNER_CAMERA_H

#include "types.h"
#include "algebra.h"

#include <SDL2/SDL.h>

typedef struct camera_t {
    Vec2 position;
    Vec2* target;
    Rect cameraView;
    Rect worldView;
} Camera;

void camera_init(Camera* self, Vec2* target);
void camera_contrain(Camera* self, Vec2* point);
void camera_update(Camera* self);

#endif