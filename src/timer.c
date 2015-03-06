#include "timer.h"
#include "entitymanager.h"

POOL_IMPLEMENTATION(Timer);

void timer_manager_init(TimerManager* self, EntityManager* entityManager) {
    self->entityManager = entityManager;
    Timer invalidTimer = { {0, 0}, 0, 0 };
    POOL_INIT(Timer)(&self->timers, TIMER_MAX_TIMERS, invalidTimer);
}

void timer_manager_terminate(TimerManager* self) {
    POOL_FREE(Timer)(&self->timers);
}

Timer* timer_manager_add(TimerManager* self, Message message, Entity target, f32 delay) {
    return timer_manager_add_interval(self, message, target, delay, 0.f);
}

Timer* timer_manager_add_interval(TimerManager* self, Message message, Entity target, f32 delay, f32 interval) {
    Timer timer = {
        { target, message },
        interval,
        delay
    };

    Timer* result = POOL_INSERT(Timer)(&self->timers, timer);

    if (message.type == MESSAGE_GENERIC_TICK) {
        MessageOnTick params = *(MessageOnTick*)message.paramBlock;
        params.timer = result;
        MESSAGE_SET_PARAM_BLOCK(result->message.message, params);
    }

    return result;
}

void timer_manager_remove(TimerManager* self, Timer* timer) {
    for (u32 i = 0; i < self->timers.capacity; ++i) {
        if (POOL_GET(Timer)(&self->timers, i) == timer) {
            POOL_REMOVE_AT(Timer)(&self->timers, i);
            break;
        }
    }
}

void timer_manager_tick(TimerManager* self, f32 dt) {
    for (u32 i = 0; i < self->timers.capacity; ++i) {
        Timer* t = POOL_GET(Timer)(&self->timers, i);
        if (t->message.target > 0) {
            t->timer -= dt;
            if (t->timer <= 0.f) {
                entities_send_message(self->entityManager, t->message.target, t->message.message);

                if (t->interval > 0.f) {
                    t->timer = t->interval;
                } else {
                    timer_manager_remove(self, t);
                }
            }
        }
    }
}
