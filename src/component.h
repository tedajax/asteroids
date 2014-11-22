#ifndef RUNNER_COMPONENT_H
#define RUNNER_COMPONENT_H

#include "core.h"
#include "messaging.h"

typedef enum component_type_e {
    COMPONENT_INVALID,

    COMPONENT_TRANSFORM,
    COMPONENT_MOVEMENT,

    COMPONENT_CONTROLLER,
    COMPONENT_BULLET_CONTROLLER,
    COMPONENT_GRAVITY,

    COMPONENT_HEALTH,
    COMPONENT_SPRITE,

    COMPONENT_BG_MANAGER,

    COMPONENT_ENEMY,

    COMPONENT_COLLIDER,

    COMPONENT_LAST,
} ComponentType;

#define REGISTER_HANDLER(type, func) ((Component*)self)->handlers[type] = func

typedef struct component_t {
    ComponentType type;
    message_cb handlers[MESSAGE_LAST];
} Component;

void component_init(Component* self, ComponentType type);
void component_free(Component* self);
void component_free_void(void* self);
void component_send_message(Component* self, Message* message);

#endif