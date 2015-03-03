#ifndef RUNNER_TIMER_H
#define RUNNER_TIMER_H

#include "types.h"
#include "gametime.h"
#include "debug.h"
#include "messaging.h"
#include "entitymanager.h"
#include "pool.h"

#define TIMER_MAX_TIMERS 128

typedef struct timer_t {
    TargetedMessage message;
    f32 interval;
    f32 timer;
} Timer;

POOL_REGISTER(Timer);

typedef struct timer_manager_t {
    EntityManager* entityManager;
    POOL(Timer) timers;
} TimerManager;

void timer_manager_init(TimerManager* self, EntityManager* entityManager);
void timer_manager_terminate(TimerManager* self);

Timer* timer_manager_add(TimerManager* self, Message message, Entity target, f32 delay);
Timer* timer_manager_add_interval(TimerManager* self, Message message, Entity target, f32 delay, f32 interval);
void timer_manager_remove(TimerManager* self, Timer* timer);

void timer_manager_tick(TimerManager* self, f32 dt);

#endif