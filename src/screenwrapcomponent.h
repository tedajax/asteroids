#ifndef ASTEROIDS_SCREEN_WRAP_COMPONENT_H
#define ASTEROIDS_SCREEN_WRAP_COMPONENT_H

#include "core.h"
#include "component.h"

typedef struct screen_wrap_component_t {
    Component super;
    f32 horizontalBuffer;
    f32 verticalBuffer;
} ScreenWrapComponent;

ScreenWrapComponent* screen_wrap_component_new(Entity entity, f32 hBuffer, f32 vBuffer);
COMPONENT_DEFINE(COMPONENT_SCREEN_WRAP);

#endif