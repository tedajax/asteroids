#include "spritecomponent.h"

SpriteComponent* sprite_component_new(Entity entity, Atlas* atlas, char* spriteName, i32 layer) {
    SpriteComponent* self = (SpriteComponent*)calloc(1, sizeof(SpriteComponent));

    component_init((Component*)self, COMPONENT_SPRITE, sizeof(SpriteComponent), entity);

    self->atlas = atlas;
    self->layer = layer;
    self->orientation = 0.f;
    self->orientationSpin = 0.f;
    self->orientToRotation = true;
    self->redTimer = 0.f;

    sprite_component_set_sprite(self, spriteName);

    return self;
}

void sprite_component_set_sprite(SpriteComponent* self, char* spriteName) {
    SpriteFrame* frame = atlas_get_frame(self->atlas, spriteName);
    ASSERT(frame, "Unable to find sprite.");
    self->currentFrame = *frame;
    self->width = (int)self->currentFrame.frame.width;
    self->height = (int)self->currentFrame.frame.height;
}

COMPONENT_DESERIALIZE(COMPONENT_SPRITE) {
    char* atlasName = CONFIG_GET(string)(config, table, "atlas");
    char* spriteName = CONFIG_GET(string)(config, table, "sprite");
    i32 layer = CONFIG_TRY_GET(i32)(config, table, "layer", 0);
    bool orientToRotation = CONFIG_TRY_GET(bool)(config, table, "orient_to_rotation", true);
    f32 orientation = CONFIG_TRY_GET(f32)(config, table, "orientation", 0.f);
    f32 orientationSpin = CONFIG_TRY_GET(f32)(config, table, "orientation_spin", 0.f);
    
    Atlas* atlas = atlas_get(atlasName);
    ASSERT(atlas, "Unable to find atlas.");

    SpriteComponent* sprite = sprite_component_new(0, atlas, spriteName, layer);
    sprite->orientToRotation = orientToRotation;
    sprite->orientation = orientation;
    sprite->orientationSpin = orientationSpin;
    return (Component*)sprite;
}

COMPONENT_FREE(COMPONENT_SPRITE) {}

COMPONENT_COPY(COMPONENT_SPRITE) {}

void sprite_component_source(SpriteComponent* self, SDL_Rect* source) {
    source->x = (int)self->currentFrame.frame.position.x;
    source->y = (int)self->currentFrame.frame.position.y;
    source->w = (int)self->currentFrame.frame.width;
    source->h = (int)self->currentFrame.frame.height;
}

void sprite_component_destination(SpriteComponent* self, TransformComponent* transform, SDL_Rect* dest) {
    f32 px = transform->position.x - globals.camera.position.x + self->currentFrame.offset.x;
    f32 py = transform->position.y - globals.camera.position.y + self->currentFrame.offset.y;
    f32 sx = transform->scale.x;
    f32 sy = transform->scale.y;

    int hw = (int)(self->width / 2.f * sx);
    int hh = (int)(self->height / 2.f * sy);
    int x = (int)(px - hw);
    int y = (int)(py - hh);
    int w = (int)(self->width * sx);
    int h = (int)(self->height * sy);

    dest->x = x;
    dest->y = y;
    dest->w = w;
    dest->h = h;
}

void sprite_component_on_collision_enter(Component* component, Message message) {
    SpriteComponent* self = (SpriteComponent*)component;
    
    self->redTimer = 0.05f;
}

int sprite_component_layer_compare(Component* c1, Component* c2) {
    return ((SpriteComponent*)c1)->layer - ((SpriteComponent*)c2)->layer;
}