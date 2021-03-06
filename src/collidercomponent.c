#include "collidercomponent.h"

ColliderComponent* collider_component_new(Entity entity, Collider* collider) {
    ColliderComponent* self = CALLOC(1, ColliderComponent);

    component_init((Component*)self, COMPONENT_COLLIDER, sizeof(ColliderComponent), entity);

    self->collider = *collider;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_COLLIDER) {
    ColliderConfig* colliderConfig = CONFIG_GET(ColliderConfig)(config, table, "collider");
    Collider collider;
    collider_init_config(&collider, 0, colliderConfig);
    return (Component*)collider_component_new(0, &collider);
}

COMPONENT_FREE(COMPONENT_COLLIDER) {
    ColliderComponent* self = (ColliderComponent*)component;
    free(self->collider.volume);
}

COMPONENT_COPY(COMPONENT_COLLIDER) {
    ColliderComponent* colliderSource = (ColliderComponent*)source;
    ColliderComponent* colliderDest = (ColliderComponent*)dest;

    size_t colliderSize = physics_volume_type_mem_size(colliderSource->collider.volume->type);
    ASSERT(colliderSize > 0, "Invalid size for collider.");
    colliderDest->collider.volume = (BoundingVolume*)malloc(colliderSize);
    bounding_volume_copy(colliderSource->collider.volume, colliderDest->collider.volume);
}

int collider_component_compare(Component* c1, Component* c2) {
    ColliderComponent* cc1 = (ColliderComponent*)c1;
    ColliderComponent* cc2 = (ColliderComponent*)c2;
    f32 diff = cc1->collider.volume->bounds.position.x - cc2->collider.volume->bounds.position.x;
    return (diff > 0.f) - (diff < 0.f);
}