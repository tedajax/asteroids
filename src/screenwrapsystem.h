#ifndef RUNNER_SCREEN_WRAP_SYSTEM_H
#define RUNNER_SCREEN_WRAP_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "screenwrapcomponent.h"

typedef struct screen_wrap_system_t {
    AspectSystem super;
} ScreenWrapSystem;

void screen_wrap_system_init(ScreenWrapSystem* self, EntityManager* entityManager);
void screen_wrap_system_update(ScreenWrapSystem* self);

#endif