#include "ufocontrollercomponent.h"

UfoControllerComponent* ufo_controller_component_new(Entity entity, UfoType ufoType, f32 speed, BulletSourceConfig* bulletSourceConfig) {
    UfoControllerComponent* self = CALLOC(1, UfoControllerComponent);

    component_init(&self->super, COMPONENT_UFO_CONTROLLER, sizeof(UfoControllerComponent), entity);

    self->ufoType = ufoType;
    self->playerTransform = NULL;
    self->fireTimer = NULL;
    self->changeDirectionTimer = NULL;
    self->movementAngle = 45.f;
    self->movementSpeed = speed;
    bullet_source_init(&self->bulletSource, bulletSourceConfig);

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_UFO_CONTROLLER) {
    char* ufoTypeStr = CONFIG_TRY_GET(string)(config, table, "type", "UFO_BIG");
    UfoType ufoType = ufo_type_parse(ufoTypeStr);
    BulletSourceConfig* bulletConfig = CONFIG_GET(BulletSourceConfig)(config, table, "bullet_source");
    f32 speed = CONFIG_GET(f32)(config, table, "speed");

    return (Component*)ufo_controller_component_new(0, ufoType, speed, bulletConfig);
}

COMPONENT_FREE(COMPONENT_UFO_CONTROLLER) { }
COMPONENT_COPY(COMPONENT_UFO_CONTROLLER) { }

UfoType ufo_type_parse(const char* str) {
    if (STR_ENUM_CMP(str, UFO_BIG)) {
        return UFO_BIG;
    } else if (STR_ENUM_CMP(str, UFO_SMALL)) {
        return UFO_SMALL;
    } else {
        ASSERT(false, "Unable to parse UfoType.");
        return 0;
    }
}