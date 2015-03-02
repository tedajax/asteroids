#include "aspectsystem.h"

void aspect_system_init(AspectSystem* self, EntityManager* entityManager, ComponentType type, u32 capacity) {
    self->entityManager = entityManager;
    self->systemType = type;

    REGISTER_SYSTEM(self->entityManager, self, capacity);

    for (u32 i = 0; i < MESSAGE_LAST; ++i) {
        self->handlers[i] = NULL;
    }
}

ComponentList* aspect_system_components(AspectSystem* self) {
    return entities_get_all_components(self->entityManager, self->systemType);
}

void aspect_system_send_message(AspectSystem* self, Entity entity, const Message message) {
    if (self->handlers[message.type]) {
        if (message.type == MESSAGE_DAMAGE) {
            int p = 0;
        }
        self->handlers[message.type](self, entity, message);
    }
}