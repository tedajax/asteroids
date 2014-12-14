#include "tween.h"
#include <math.h>

#pragma region Tween Functions

f32 tween_linear(f32 t, f32 i, f32 f, f32 d) {
    return (f - i) * t / d + i;
}

f32 tween_ease_in_quad(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    return (f - i) * t * t + i;
}

f32 tween_ease_out_quad(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    return -(f - i) * (t - 2.f) + i;
}

f32 tween_ease_in_out_quad(f32 t, f32 i, f32 f, f32 d) {
    t /= (d / 2.f);
    if (t < 1.f) {
        return (f - i) / 2.f * t * t + i;
    }
    t -= 1.f;
    return -(f - i) / 2 * (t * (t - 2) - 1) + i;
}

f32 tween_ease_in_sin(f32 t, f32 i, f32 f, f32 d) {
    return -(f - i) * sinf(t / d * TWO_PI) + i;
}

f32 tween_ease_out_sin(f32 t, f32 i, f32 f, f32 d) {
    return (f - i) * sinf(t / d * TWO_PI) + i;
}

f32 tween_ease_in_out_sin(f32 t, f32 i, f32 f, f32 d) {
    return -(f - i) / 2.f * (cosf(PI * t / d) - 1.f) + i;
}

f32 tween_ease_in_expo(f32 t, f32 i, f32 f, f32 d) {
    return (f - i) * powf(2.f, 10.f * (t / d - 1.f)) + i;
}

f32 tween_ease_out_expo(f32 t, f32 i, f32 f, f32 d) {
    return (f - i) * (-powf(2.f, -10.f * t / d) + 1.f) + i;
}

f32 tween_ease_in_out_expo(f32 t, f32 i, f32 f, f32 d) {
    t /= (d / 2.f);
    if (t < 1.f) {
        return (f - i) / 2.f * powf(2.f, 10.f * (t - 1.f)) + i;
    }
    t -= 1.f;
    return (f - i) / 2.f * (-powf(2.f, -10.f * t) + 2.f) + i;
}

f32 tween_ease_in_circ(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    return -(f - i) * (sqrtf(1.f - t * t) - 1.f) + i;
}

f32 tween_ease_out_circ(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    t -= 1.f;
    return (f - i) * sqrtf(1.f - t * t) + i;
}

f32 tween_ease_in_out_circ(f32 t, f32 i, f32 f, f32 d) {
    t /= (d / 2.f);
    if (t < 1.f) {
        return -(f - i) / 2.f * (sqrtf(1.f - t * t) - 1.f) + i;
    }
    t -= 2.f;
    return (f - i) / 2.f * (sqrtf(1.f - t * t) + 1.f) + i;
}

f32 tween_bounce_in(f32 t, f32 i, f32 f, f32 d) {
    return (f - i) - tween_bounce_out(d - t, 0.f, f, d) + i;
}

f32 tween_bounce_out(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    if (t < (1.f / 2.75f)) {
        return (f - i) * (7.5625f * t * t) + i;
    } else if (t < (2.f / 2.75f)) {
        t -= (1.5f / 2.75f);
        return (f - i) * (7.5625f * t * t + .75f) + i;
    } else if (t < (2.5f / 2.75f)) {
        t -= (2.25f / 2.75f);
        return (f - i) * (7.5625f * t * t + .9375f) + i;
    } else {
        t -= (2.625f / 2.75f);
        return (f - i) * (7.5625f * t * t + .984375f) + i;
    }
}

f32 tween_bounce_in_out(f32 t, f32 i, f32 f, f32 d) {
    if (t < (d / 2.f)) {
        return tween_bounce_in(t * 2.f, 0.f, f, d) * 0.5f + i;
    } else {
        return tween_bounce_out(t * 2.f - d, 0.f, f, d) * 0.5f + (f - i) * 0.5f + i;
    }
}

f32 tween_sin_wave(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    return sinf(t * TWO_PI) * ((f - i) / 2.f) + i + ((f - i) / 2.f);
}

f32 tween_cos_wave(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    return cosf(t * TWO_PI) * ((f - i) / 2.f) + i + ((f - i) / 2.f);
}

f32 tween_parabolic(f32 t, f32 i, f32 f, f32 d) {
    return ((-4.f * (i + (f - i))) / (d * d)) * t * (t - d);
}

#pragma endregion

void tween_init(Tween* self, f32 start, f32 end, f32 duration, u32 loops, tween_func tweenFunc) {
    self->start = start;
    self->end = end;
    self->duration = duration;
    self->loops = loops;

    self->tweenFunc = tweenFunc;

    self->time = 0.f;
    self->loopsLeft = loops;
}

void tween_zero(Tween* self) {
    self->start = 0.f;
    self->end = 0.f;
    self->duration = 0.f;
    self->loops = 0;

    self->timescale = 0.f;

    self->tweenFunc = NULL;

    self->time = 0.f;
    self->loopsLeft = 0;

    self->enabled = false;
}

f32 tween_evaluate(Tween* self) {
    ASSERT(self->enabled, "Attempt to evaluate inactive tween.");
    ASSERT(self->tweenFunc, "Attempt to evaluate tween with no tweening function.");

    f32 i = self->start;
    f32 f = self->end;

    if (self->timescale < 0.f) {
        i = self->end;
        f = self->start;
    }

    return self->tweenFunc(self->time, i, f, self->duration);
}

void tween_update(Tween* self, f32 dt) {
    f32 ts = fabsf(self->timescale);

    self->time += dt * ts;

    if (self->time > self->duration) {
        if (self->loopsLeft > 0) {
            if (self->loopsLeft != TWEEN_LOOP_INFINITE) {
                --self->loopsLeft;
            }
            self->time -= self->duration;
        } else {
            self->enabled = false;
            self->time = self->duration;
        }
    }
}

void tween_play(Tween* self) {
    self->timescale = 1.f;
}

void tween_pause(Tween* self) {
    self->timescale = 0.f;
}

void tween_stop(Tween* self) {
    self->timescale = 0.f;
    self->time = 0.f;
}

void tween_reset(Tween* self) {
    self->time = 0.f;
    self->loopsLeft = self->loops;
}

void tween_manager_init(TweenManager* self, u32 capacity) {
    self->capacity = capacity;
    
    self->tweens = CALLOC(self->capacity, Tween);
    self->freeIndices = CALLOC(self->capacity, u32);
    
    for (u32 i = self->capacity; i > 0; --i) {
        self->freeIndices[i - 1] = i;
        tween_zero(&self->tweens[i - 1]);
    }

    self->freeHead = self->capacity - 1;
}

void tween_manager_update(TweenManager* self, f32 dt) {
    for (u32 i = 0; i < self->capacity; ++i) {
        Tween* tween = &self->tweens[i];
        if (tween->enabled) {
            tween_update(tween, dt);

            // If the most recent update finished
            if (!tween->enabled) {
                //push the index back into the free indices stack
                ASSERT(self->freeHead < self->capacity - 1, "No room in free indices stack.");
                tween_zero(tween);
                ++self->freeHead;
                self->freeIndices[self->freeHead] = i;
            }
        }
    }
}

void tween_manager_remove(TweenManager* self, Tween* tween) {
    for (u32 i = 0; i < self->capacity; ++i) {
        if (tween == &self->tweens[i]) {
            for (u32 j = 0; j < self->freeHead; ++j) {
                if (self->freeIndices[j] == i) {
                    // this tween has already been removed
                    return;
                }
            }

            ASSERT(self->freeHead < self->capacity - 1, "No room in free indices stack.");

            tween_zero(tween);
            ++self->freeHead;
            self->freeIndices[self->freeHead] = i;

            return;
        }
    }
}

Tween* tween_manager_create(TweenManager* self, f32 start, f32 end, f32 duration, u32 loops, tween_func tweenFunc) {
    ASSERT(self->freeHead > 0, "Reached maximum tween capacity.  Consider increasing capacity of this tween manager.");
    
    u32 index = self->freeIndices[self->freeHead];
    --self->freeHead;
    Tween* tween = &self->tweens[index];

    tween_init(tween, start, end, duration, loops, tweenFunc);

    return tween;
}