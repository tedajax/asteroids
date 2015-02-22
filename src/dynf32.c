#include "dynf32.h"
#include "tween.h"

void dynf32_zero(dynf32* self) {
    self->value = 0.f;
    self->tween = NULL;
    self->tweenConfig = NULL;
}

dynf32 dynf32_value(f32 value) {
    dynf32 result;
    result.value = value;
    result.tween = NULL;
    result.tweenConfig = NULL;
    result.type = DYN_F32_VALUE;
    return result;
}

void dynf32_release(dynf32* self) {
    if (self->type == DYN_F32_TWEEN && self->tween) {
        tween_release(self->tween);
    }
}

void dynf32_start(dynf32* self, TweenManager* tweenManager) {
    if (self->type != DYN_F32_TWEEN) {
        return;
    }

    if (self->tween != NULL) {
        dynf32_release(self);
    }

    self->tween = tween_manager_create(tweenManager, self->tweenConfig);
    tween_play(self->tween);
}

void dynf32_copy(const dynf32* source, dynf32* dest) {
    dest->type = source->type;
    dest->tweenConfig = source->tweenConfig;
    dest->value = source->value;
    dest->tween = NULL;
}

f32 dynf32_get(dynf32* self) {
    switch (self->type) {
        default:
        case DYN_F32_VALUE:
            return self->value;

        case DYN_F32_TWEEN:
            return tween_evaluate(self->tween);
    }
}

void dynf32_set(dynf32* self, f32 value) {
    if (self->type == DYN_F32_TWEEN) {
        if (self->tween) {
            tween_release(self->tween);
        }
        self->tween = NULL;
        self->tweenConfig = NULL;
        self->type = DYN_F32_VALUE;
    }
    self->value = value;
}

void dynf32_play(dynf32* self) {
    if (self->tween) {
        tween_play(self->tween);
    }
}

void dynf32_pause(dynf32* self) {
    if (self->tween) {
        tween_pause(self->tween);
    }
}

///////////////////////////////
////////// DynFloat2 //////////
///////////////////////////////

void dynfloat2_zero(DynFloat2* self) {
    dynf32_zero(&self->x);
    dynf32_zero(&self->y);
}

DynFloat2 dynfloat2_create(dynf32 x, dynf32 y) {
    DynFloat2 result;
    dynf32_copy(&x, &result.x);
    dynf32_copy(&y, &result.y);
    return result;
}

void dynfloat2_release(DynFloat2* self) {
    dynf32_release(&self->x);
    dynf32_release(&self->y);
}

void dynfloat2_start(DynFloat2* self, TweenManager* tweenManager) {
    dynf32_start(&self->x, tweenManager);
    dynf32_start(&self->y, tweenManager);
}

void dynfloat2_copy(const DynFloat2* source, DynFloat2* dest) {
    dynf32_copy(&source->x, &dest->x);
    dynf32_copy(&source->y, &dest->y);
}

f32 dynfloat2_get_x(DynFloat2* self) {
    return dynf32_get(&self->x);
}

f32 dynfloat2_get_y(DynFloat2* self) {
    return dynf32_get(&self->y);
}

void dynfloat2_set_x(DynFloat2* self, f32 value) {
    dynf32_set(&self->x, value);
}

void dynfloat2_set_y(DynFloat2* self, f32 value) {
    dynf32_set(&self->y, value);
}

void dynfloat2_play(DynFloat2* self) {
    dynf32_play(&self->x);
    dynf32_play(&self->y);
}

void dynfloat2_pause(DynFloat2* self) {
    dynf32_pause(&self->x);
    dynf32_pause(&self->y);
}

///////////////////////////////
////////// DynFloat3 //////////
///////////////////////////////

void dynfloat3_zero(DynFloat3* self) {
    dynf32_zero(&self->x);
    dynf32_zero(&self->y);
    dynf32_zero(&self->z);
}

DynFloat3 dynfloat3_create(dynf32 x, dynf32 y, dynf32 z) {
    DynFloat3 result;
    dynf32_copy(&x, &result.x);
    dynf32_copy(&y, &result.y);
    dynf32_copy(&z, &result.z);
    return result;
}

void dynfloat3_release(DynFloat3* self) {
    dynf32_release(&self->x);
    dynf32_release(&self->y);
    dynf32_release(&self->z);
}

void dynfloat3_start(DynFloat3* self, TweenManager* tweenManager) {
    dynf32_start(&self->x, tweenManager);
    dynf32_start(&self->y, tweenManager);
    dynf32_start(&self->z, tweenManager);
}

void dynfloat3_copy(const DynFloat3* source, DynFloat3* dest) {
    dynf32_copy(&source->x, &dest->x);
    dynf32_copy(&source->y, &dest->y);
    dynf32_copy(&source->z, &dest->z);
}

f32 dynfloat3_get_x(DynFloat3* self) {
    return dynf32_get(&self->x);
}

f32 dynfloat3_get_y(DynFloat3* self) {
    return dynf32_get(&self->y);
}

f32 dynfloat3_get_z(DynFloat3* self) {
    return dynf32_get(&self->z);
}

void dynfloat3_set_x(DynFloat3* self, f32 value) {
    dynf32_set(&self->x, value);
}

void dynfloat3_set_y(DynFloat3* self, f32 value) {
    dynf32_set(&self->y, value);
}

void dynfloat3_set_z(DynFloat3* self, f32 value) {
    dynf32_set(&self->z, value);
}

void dynfloat3_play(DynFloat3* self) {
    dynf32_play(&self->x);
    dynf32_play(&self->y);
    dynf32_play(&self->z);
}

void dynfloat3_pause(DynFloat3* self) {
    dynf32_pause(&self->x);
    dynf32_pause(&self->y);
    dynf32_pause(&self->z);
}


///////////////////////////////
////////// DynFloat4 //////////
///////////////////////////////

void dynfloat4_zero(DynFloat4* self) {
    dynf32_zero(&self->x);
    dynf32_zero(&self->y);
    dynf32_zero(&self->z);
    dynf32_zero(&self->w);
}

DynFloat4 dynfloat4_create(dynf32 x, dynf32 y, dynf32 z, dynf32 w) {
    DynFloat4 result;
    dynf32_copy(&x, &result.x);
    dynf32_copy(&y, &result.y);
    dynf32_copy(&z, &result.z);
    dynf32_copy(&w, &result.w);
    return result;
}

void dynfloat4_release(DynFloat4* self) {
    dynf32_release(&self->x);
    dynf32_release(&self->y);
    dynf32_release(&self->z);
    dynf32_release(&self->w);
}

void dynfloat4_start(DynFloat4* self, TweenManager* tweenManager) {
    dynf32_start(&self->x, tweenManager);
    dynf32_start(&self->y, tweenManager);
    dynf32_start(&self->z, tweenManager);
    dynf32_start(&self->w, tweenManager);
}

void dynfloat4_copy(const DynFloat4* source, DynFloat4* dest) {
    dynf32_copy(&source->x, &dest->x);
    dynf32_copy(&source->y, &dest->y);
    dynf32_copy(&source->z, &dest->z);
    dynf32_copy(&source->w, &dest->w);
}

f32 dynfloat4_get_x(DynFloat4* self) {
    return dynf32_get(&self->x);
}

f32 dynfloat4_get_y(DynFloat4* self) {
    return dynf32_get(&self->y);
}

f32 dynfloat4_get_z(DynFloat4* self) {
    return dynf32_get(&self->z);
}

f32 dynfloat4_get_w(DynFloat4* self) {
    return dynf32_get(&self->w);
}

void dynfloat4_set_x(DynFloat4* self, f32 value) {
    dynf32_set(&self->x, value);
}

void dynfloat4_set_y(DynFloat4* self, f32 value) {
    dynf32_set(&self->y, value);
}

void dynfloat4_set_z(DynFloat4* self, f32 value) {
    dynf32_set(&self->z, value);
}

void dynfloat4_set_w(DynFloat4* self, f32 value) {
    dynf32_set(&self->w, value);
}

void dynfloat4_play(DynFloat4* self) {
    dynf32_play(&self->x);
    dynf32_play(&self->y);
    dynf32_play(&self->z);
    dynf32_play(&self->w);
}

void dynfloat4_pause(DynFloat4* self) {
    dynf32_pause(&self->x);
    dynf32_pause(&self->y);
    dynf32_pause(&self->z);
    dynf32_pause(&self->w);
}