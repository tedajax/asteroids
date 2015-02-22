#ifndef RUNNER_TWEEN_H
#define RUNNER_TWEEN_H

#include "types.h"
#include "debug.h"
#include "config.h"
#include "hashtable.h"

// Tween function take the form
// f32 tween_function(f32 t, f32 i, f32 f, f32 d)
// where:
//      t - current time of the tween in seconds.
//      i - initial value.
//      f - desired final value.
//      d - total duration of the tween in seconds.
//
typedef f32(*tween_func)(f32, f32, f32, f32);

f32 tween_linear(f32 t, f32 i, f32 f, f32 d);
f32 tween_start_value(f32 t, f32 i, f32 f, f32 d);
f32 tween_end_value(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_quad(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_out_quad(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_out_quad(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_sin(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_out_sin(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_out_sin(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_expo(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_out_expo(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_out_expo(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_circ(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_out_circ(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_out_circ(f32 t, f32 i, f32 f, f32 d);
f32 tween_bounce_in(f32 t, f32 i, f32 f, f32 d);
f32 tween_bounce_out(f32 t, f32 i, f32 f, f32 d);
f32 tween_bounce_in_out(f32 t, f32 i, f32 f, f32 d);
f32 tween_sin_wave(f32 t, f32 i, f32 f, f32 d);
f32 tween_cos_wave(f32 t, f32 i, f32 f, f32 d);
f32 tween_parabolic(f32 t, f32 i, f32 f, f32 d);
f32 tween_random(f32 t, f32 i, f32 f, f32 d);

Hashtable tweenFunctions;

#define TWEEN_REGISTER_FUNCTION(func) hashtable_insert(&tweenFunctions, #func, func)
#define TWEEN_GET_FUNCTION(func) (tween_func)hashtable_get(&tweenFunctions, func)

#define TWEEN_REGISTER_ALL()							\
MULTILINE_MACRO_BEGIN()									\
    hashtable_init(&tweenFunctions, 32, NULL);		\
    TWEEN_REGISTER_FUNCTION(tween_linear);			\
    TWEEN_REGISTER_FUNCTION(tween_start_value);     \
    TWEEN_REGISTER_FUNCTION(tween_end_value);		\
    TWEEN_REGISTER_FUNCTION(tween_ease_in_quad);	\
    TWEEN_REGISTER_FUNCTION(tween_ease_out_quad);	\
    TWEEN_REGISTER_FUNCTION(tween_ease_in_out_quad);\
    TWEEN_REGISTER_FUNCTION(tween_ease_in_sin);		\
    TWEEN_REGISTER_FUNCTION(tween_ease_out_sin);	\
    TWEEN_REGISTER_FUNCTION(tween_ease_in_out_sin);	\
    TWEEN_REGISTER_FUNCTION(tween_ease_in_expo);	\
    TWEEN_REGISTER_FUNCTION(tween_ease_out_expo);	\
    TWEEN_REGISTER_FUNCTION(tween_ease_in_out_expo);\
    TWEEN_REGISTER_FUNCTION(tween_ease_in_circ);	\
    TWEEN_REGISTER_FUNCTION(tween_ease_out_circ);	\
    TWEEN_REGISTER_FUNCTION(tween_ease_in_out_circ);\
    TWEEN_REGISTER_FUNCTION(tween_bounce_in);		\
    TWEEN_REGISTER_FUNCTION(tween_bounce_out);		\
    TWEEN_REGISTER_FUNCTION(tween_bounce_in_out);	\
    TWEEN_REGISTER_FUNCTION(tween_sin_wave);		\
    TWEEN_REGISTER_FUNCTION(tween_cos_wave);		\
    TWEEN_REGISTER_FUNCTION(tween_parabolic);       \
    TWEEN_REGISTER_FUNCTION(tween_random);          \
MULTILINE_MACRO_END()

enum tween_loop_e {
    TWEEN_LOOP_NONE = 0,
    TWEEN_LOOP_INFINITE = 0xFFFFFFFF,
};

typedef enum tween_state_e {
    TWEEN_STATE_FREE,   // available for the tween manager to use for a new tween
    TWEEN_STATE_ACTIVE, // tween is currently running
    TWEEN_STATE_DESTROY // tween is running but is done and waiting to be set to FREE
} TweenState;

typedef struct tween_t {
    f32 start; // value at the start
    f32 end; // value at the end
    f32 duration; // duration in seconds
    u32 loops; // number of times to loop the tween, 0 = don't loop, 0xFFFFFFFF = loop infinitely

    f32 timescale; // multiply rate of time by this value, 1 - normal, 2 - double speed, 0.5 - half speed, etc...

    tween_func tweenFunc; // which tweenFunction to use.

    f32 time;
    u32 loopsLeft;        // how many loops are left

    TweenState state;
} Tween;

void tween_init(Tween* self, TweenConfig* config);
void tween_zero(Tween* self);
f32 tween_evaluate(Tween* self);
void tween_update(Tween* self, f32 dt);
void tween_play(Tween* self);
void tween_pause(Tween* self);
void tween_stop(Tween* self);
void tween_reset(Tween* self);
void tween_release(Tween* self);

typedef struct tween_manager_t {
    Tween* tweens;
    u32* freeIndices;
    u32 capacity;
    u32 freeHead;
    u32 count;
} TweenManager;

void tween_manager_init(TweenManager* self, u32 capacity);
void tween_manager_terminate(TweenManager* self);
void tween_manager_update(TweenManager* self, f32 dt);
Tween* tween_manager_create(TweenManager* self, TweenConfig* config);
void tween_manager_remove(TweenManager* self, Tween* tween);
tween_func tween_parse(char* tweenName);

#endif