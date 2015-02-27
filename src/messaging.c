#include "messaging.h"

void messaging_system_init(MessagingSystem* self) {
    for (u32 i = 0; i < MESSAGE_LAST; ++i) {
        self->subscriberLists[i].count = 0;
        u32 capacity = 32;
        self->subscriberLists[i].capacity = capacity;
        self->subscriberLists[i].subscribers = CALLOC(capacity, Entity);
    }
}

void messaging_system_terminate(MessagingSystem* self) {
    for (u32 i = 0; i < MESSAGE_LAST; ++i) {
        free(self->subscriberLists[i].subscribers);
    }
}

void messaging_system_add_subscriber(MessagingSystem* self, MessageType type, Entity entity) {
    SubscriberList* list = &self->subscriberLists[type];
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->subscribers = (Entity*)realloc(list->subscribers, list->capacity * sizeof(Entity));
    }

    list->subscribers[list->count] = entity;
    ++list->count;
}

void message_queue_init(MessageQueue* self) {
    self->size = 0;
    self->head = 0;
    self->tail = 0;
}

void message_queue_push(MessageQueue* self, TargetedMessage message) {
    ASSERT(self->tail < MESSAGE_QUEUE_CAPACITY, "Reached capacity of message queue.");
    
    self->messages[self->tail] = message;
    ++self->size;
    ++self->tail;
}

TargetedMessage message_queue_pop(MessageQueue* self) {
    ASSERT(self->size > 0, "Nothing to pop");

    TargetedMessage result = self->messages[self->head];

    ++self->head;

    --self->size;
    if (self->size == 0) {
        self->head = 0;
        self->tail = 0;
    }

    return result;
}

TargetedMessage message_queue_peek(MessageQueue* self, i32 index) {
    i32 trueIndex = self->head + index;

    ASSERT(trueIndex < MESSAGE_QUEUE_CAPACITY, "Index out of range.");

    return self->messages[trueIndex];
}

void message_event_queue_init(MessageEventQueue* self) {
    message_queue_init(&self->messageQueue);
    message_queue_init(&self->immediateQueue);
    self->processingLock = false;
    self->processingType = MESSAGE_LAST;
}

void message_event_queue_push(MessageEventQueue* self, Entity entity, Message message) {
    if (!self->processingLock) {
        message_event_queue_push_deferred(self, entity, message);
        return;
    }

    TargetedMessage targetedMessage = { entity, message };
    message_queue_push(&self->immediateQueue, targetedMessage);
}

void message_event_queue_push_deferred(MessageEventQueue* self, Entity entity, Message message) {
    TargetedMessage targetedMessage = { entity, message };
    message_queue_push(&self->messageQueue, targetedMessage);
}

TargetedMessage message_event_queue_pop(MessageEventQueue* self) {
    if (self->immediateQueue.size <= 0) {
        TargetedMessage result = message_queue_pop(&self->messageQueue);
        self->processingType = result.message.type;
        return result;
    } else {
        if (self->messageQueue.size > 0 && message_queue_peek(&self->messageQueue, 0).message.type != self->processingType) {
            return message_queue_pop(&self->messageQueue);
        } else {
            return message_queue_pop(&self->immediateQueue);
        }
    }
}
