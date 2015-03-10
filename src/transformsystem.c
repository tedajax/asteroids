#include "transformsystem.h"

void transform_system_init(TransformSystem* self, GameScene* scene) {
    aspect_system_init(&self->super, scene, COMPONENT_TRANSFORM, MAX_ENTITIES);
}