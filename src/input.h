#ifndef ASTEROIDS_INPUT_H
#define ASTEROIDS_INPUT_H

#include "memory.h"

#include <SDL2/SDL.h>

#include "types.h"
#include "debug.h"

#include <string.h>

bool oldKeyStates[SDL_NUM_SCANCODES];
bool newKeyStates[SDL_NUM_SCANCODES];

void input_initialize();

void input_update();
void input_handle_event(SDL_KeyboardEvent* event);

bool input_key(u32 scancode);
bool input_key_down(u32 scancode);
bool input_key_up(u32 scancode);

#endif