#include "screenwrapsystem.h"

void screen_wrap_system_init(ScreenWrapSystem* self, GameScene* scene) {
    aspect_system_init(&self->super, scene, COMPONENT_SCREEN_WRAP, MAX_ENTITIES);
}

void screen_wrap_system_update(ScreenWrapSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);

        ScreenWrapComponent* screenWrap =
            (ScreenWrapComponent*)GET_SYSTEM_COMPONENT(i);

        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        REQUIRED_COMPONENTS(screenWrap, transform);

        f32 left = -screenWrap->horizontalBuffer;
        f32 right = screenWrap->horizontalBuffer + globals.world.width;
        f32 top = -screenWrap->verticalBuffer;
        f32 bottom = screenWrap->verticalBuffer + globals.world.height;

        if (transform->position.x < left) {
            transform->position.x = right;
        } else if (transform->position.x > right) {
            transform->position.x = left;
        }

        if (transform->position.y < top) {
            transform->position.y = bottom;
        } else if (transform->position.y > bottom) {
            transform->position.y = top;
        }
    }
}