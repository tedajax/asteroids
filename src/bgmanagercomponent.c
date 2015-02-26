#include "bgmanagercomponent.h"
#include "prefab.h"
#include "spritecomponent.h"
#include "entitymanager.h"
#include "gamescene.h"
#include "game.h"

BgManagerComponent* bg_manager_component_new(Entity entity) {
    BgManagerComponent* self = CALLOC(1, BgManagerComponent);

    component_init((Component*)self, COMPONENT_BG_MANAGER, sizeof(BgManagerComponent), entity);

    self->tileWidth = 0;
    self->tileHeight = 0;

    /*u32 x = globals.world.width / self->tileWidth + 2;
    u32 y = globals.world.height / self->tileHeight + 2;*/

    self->rows = 0;
    self->columns = 0;

    self->capacity = 0;
    self->transforms = NULL;
    self->count = 0;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_BG_MANAGER) {
    char* prefabName = CONFIG_GET(string)(config, table, "prefab");
    Prefab* prefab = prefab_get(prefabName);
    ASSERT(prefab, "Unable to load prefab.");

    BgManagerComponent* bg = bg_manager_component_new(0);
    bg->tilePrefab = prefab;

    return (Component*)bg;
}

COMPONENT_FREE(COMPONENT_BG_MANAGER) {
    BgManagerComponent* self = (BgManagerComponent*)component;
    free(self->transforms);
}

COMPONENT_COPY(COMPONENT_BG_MANAGER) {
    BgManagerComponent* bgSrc = (BgManagerComponent*)source;
    BgManagerComponent* bgDest = (BgManagerComponent*)dest;

    bgDest->transforms = CALLOC(bgDest->capacity, TransformComponent*);
    memcpy(bgDest->transforms, bgSrc->transforms, bgSrc->count * sizeof(TransformComponent*));
}

bool bg_manager_component_add_entity(BgManagerComponent* self, EntityManager* entityManager, Entity entity) {
    if (self->count >= self->capacity) {
        return false;
    }

    TransformComponent* transform =
        (TransformComponent*)entities_get_component(entityManager,
        COMPONENT_TRANSFORM,
        entity);

    ASSERT(transform, "");

    self->transforms[self->count] = transform;
    ++self->count;

    return true;
}