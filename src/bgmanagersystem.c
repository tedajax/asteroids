#include "bgmanagersystem.h"
#include "spritecomponent.h"
#include "prefab.h"

void bg_manager_system_init(BgManagerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_BG_MANAGER, 1);
}

void bg_manager_system_start(BgManagerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        BgManagerComponent* bg = (BgManagerComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(bg);

        u32 width = 0, height = 0;
        {
            Entity entity = prefab_instantiate(bg->tilePrefab);
            SpriteComponent* sprite = (SpriteComponent*)entities_get_component(self->super.entityManager,
                COMPONENT_SPRITE,
                entity);
            width = (u32)floorf(sprite->currentFrame.frame.width);
            height = (u32)floorf(sprite->currentFrame.frame.height);
            entities_remove_entity(self->super.entityManager, entity);
        }

        bg->tileWidth = width;
        bg->tileHeight = height;

        bg->rows = globals.world.height / bg->tileHeight + 2;
        bg->columns = globals.world.width / bg->tileWidth + 2;

        bg->capacity = bg->rows * bg->columns;
        bg->transforms = CALLOC(bg->capacity, TransformComponent*);

        for (u32 j = 0; j < bg->capacity; ++j) {
            Entity entity = prefab_instantiate(bg->tilePrefab);
            bg_manager_component_add_entity(bg, self->super.entityManager, entity);

            TransformComponent* tx = bg->transforms[j];

            //todo place correctly based on camera and shit
            f32 x = (f32)((j % bg->columns) * bg->tileWidth);
            f32 y = (f32)((j / bg->columns) * bg->tileHeight);
            vec2_set(&tx->position, x, y);
        }
    }
}

void bg_manager_system_update(BgManagerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        BgManagerComponent* bg = (BgManagerComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(bg);

        f32 w = (f32)bg->tileWidth;
        f32 h = (f32)bg->tileHeight;
        f32 hw = (f32)bg->tileWidth / 2.f;
        f32 hh = (f32)bg->tileHeight / 2.f;

        f32 left = globals.camera.position.x - w + hw;
        f32 right = globals.camera.position.x + globals.world.width + hw;
        f32 top = globals.camera.position.y - h + hh;
        f32 bottom = globals.camera.position.y + globals.world.height + hh;

        for (u32 j = 0; j < bg->count; ++j) {
            TransformComponent* tx = bg->transforms[j];

            if (tx->position.x < left) {
                tx->position.x += (f32)(bg->columns * bg->tileWidth);
            } else if (tx->position.x > right) {
                tx->position.x -= (f32)(bg->columns * bg->tileWidth);
            }

            if (tx->position.y < top) {
                tx->position.y += (f32)(bg->rows * bg->tileHeight);
            } else if (tx->position.y > bottom) {
                tx->position.y -= (f32)(bg->rows * bg->tileHeight);
            }
        }
    }
}