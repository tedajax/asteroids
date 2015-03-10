#include "input.h"

void input_initialize(InputManager* self) {
    for (u32 i = 0; i < SDL_NUM_SCANCODES; ++i) {
        self->oldKeyStates[i] = false;
        self->newKeyStates[i] = false;
    }
}

void input_update(InputManager* self) {
    memcpy(&self->oldKeyStates[0], &self->newKeyStates[0], SDL_NUM_SCANCODES);
}

void input_handle_event(InputManager* self, SDL_KeyboardEvent* event) {
    bool down = (event->type == SDL_KEYDOWN);
    self->newKeyStates[event->keysym.scancode] = down;
}

bool input_key(InputManager* self, u32 scancode) {
    ASSERT(scancode < SDL_NUM_SCANCODES, "Scancode out of range.");

    return self->newKeyStates[scancode];
}

bool input_key_down(InputManager* self, u32 scancode) {
    ASSERT(scancode < SDL_NUM_SCANCODES, "Scancode out of range.");

    return self->newKeyStates[scancode] && !self->oldKeyStates[scancode];
}

bool input_key_up(InputManager* self, u32 scancode) {
    ASSERT(scancode < SDL_NUM_SCANCODES, "Scancode out of range.");

    return !self->newKeyStates[scancode] && self->oldKeyStates[scancode];
}