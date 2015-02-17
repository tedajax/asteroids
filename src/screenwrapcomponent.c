#include "screenwrapcomponent.h"

ScreenWrapComponent* screen_wrap_component_new(Entity entity, f32 hBuffer, f32 vBuffer) {
    ScreenWrapComponent* self = CALLOC(1, ScreenWrapComponent);

    component_init((Component*)self, COMPONENT_SCREEN_WRAP, sizeof(ScreenWrapComponent), entity);

    self->horizontalBuffer = hBuffer;
    self->verticalBuffer = vBuffer;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_SCREEN_WRAP) {
    f32 hBuffer = CONFIG_GET(float)(config, table, "horizontal_buffer");
    f32 vBuffer = CONFIG_GET(float)(config, table, "vertical_buffer");
    return (Component*)screen_wrap_component_new(0, hBuffer, vBuffer);
}

COMPONENT_FREE(COMPONENT_SCREEN_WRAP) {}

COMPONENT_COPY(COMPONENT_SCREEN_WRAP) {}