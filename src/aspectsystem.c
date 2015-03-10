#include "aspectsystem.h"
#include "gamescene.h"

void aspect_system_init(AspectSystem* self, GameScene* scene, ComponentType type, u32 capacity) {
    self->scene = scene;
    self->entityManager = scene->entityManager;
    self->input = &scene->input;
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
        self->handlers[message.type](self, entity, message);
    }
}