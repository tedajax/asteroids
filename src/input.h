#ifndef ASTEROIDS_INPUT_H
#define ASTEROIDS_INPUT_H

#include "memory.h"

#include <SDL2/SDL.h>

#include "types.h"
#include "debug.h"

#include <string.h>

typedef struct input_manager_t {
    bool oldKeyStates[SDL_NUM_SCANCODES];
    bool newKeyStates[SDL_NUM_SCANCODES];
} InputManager;

void input_initialize(InputManager* self);

void input_update(InputManager* self);
void input_handle_event(InputManager* self, SDL_KeyboardEvent* event);

bool input_key(InputManager* self, u32 scancode);
bool input_key_down(InputManager* self, u32 scancode);
bool input_key_up(InputManager* self, u32 scancode);

#endif