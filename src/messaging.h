#ifndef RUNNER_MESSAGING_H
#define RUNNER_MESSAGING_H

#include "core.h"

//Forward declarations
typedef struct component_t Component;
typedef struct aspect_system_t AspectSystem;

// Each comment refers to the types of the params in the message.
typedef enum message_type_e {
    // Entity source, f32 amount, NULL
    MESSAGE_DAMAGE,

    // Entity other, NULL, NULL
    MESSAGE_ON_COLLISION_ENTER,

    // Entity other, NULL, NULL
    MESSAGE_ON_COLLISION_STAY,

    // Entity other, NULL, NULL
    MESSAGE_ON_COLLISION_EXIT,

    // Entity created, NULL, NULL
    MESSAGE_ENTITY_ADDED,

    // Entity removed, NULL, NULL
    MESSAGE_ENTITY_REMOVED,

    // N/A
    MESSAGE_LAST
} MessageType;

#define MESSAGE_PARAM_BLOCK_SIZE 64
#define MESSAGE_QUEUE_CAPACITY 64
#define MESSAGE_IMMEDIATE_QUEUE_CAPACITY 64

typedef struct message_on_collision_params_t {
    Entity other;
} MessageOnCollisionParams;

typedef struct message_on_damage_params_t {
    i32 damage;
    Vec2 direction;
} MessageOnDamageParams;

typedef struct message_on_added_params_t {
    Entity addedEntity;
} MessageOnAddedParams;

typedef struct message_on_removed_params_t {
    Entity removedEntity;
} MessageOnRemovedParams;

// TODO: Figure out some way of enforcing a schema on message params.  Right now it's really just built on trust.
typedef struct message_t {
    MessageType type;
    u8 paramBlock[MESSAGE_PARAM_BLOCK_SIZE];
} Message;

typedef struct targeted_message_t {
    Entity target;
    Message message;
} TargetedMessage;

typedef struct message_queue_t {
    TargetedMessage messages[MESSAGE_QUEUE_CAPACITY];
    i32 size;
    i32 head;
    i32 tail;
} MessageQueue;

typedef struct message_subscriber_list_t {
    u32 count;
    u32 capacity;
    Entity* subscribers;
} SubscriberList;

typedef struct messaging_system_t {
    SubscriberList subscriberLists[MESSAGE_LAST];
} MessagingSystem;

typedef struct message_event_queue_t {
    MessageQueue messageQueue;
    MessageQueue immediateQueue;
    MessageType processingType;
    bool processingLock;
} MessageEventQueue;

void messaging_system_init(MessagingSystem* self);
void messaging_system_terminate(MessagingSystem* self);
void messaging_system_add_subscriber(MessagingSystem* self, MessageType type, Entity entity);

void message_queue_init(MessageQueue* self);

void message_queue_push(MessageQueue* self, TargetedMessage message);
TargetedMessage message_queue_pop(MessageQueue* self);
TargetedMessage message_queue_peek(MessageQueue* self, i32 index);

void message_event_queue_init(MessageEventQueue* self);
void message_event_queue_push(MessageEventQueue* self, Entity entity, Message message);
void message_event_queue_push_deferred(MessageEventQueue* self, Entity entity, Message message);
TargetedMessage message_event_queue_pop(MessageEventQueue* self);

static inline i32 message_event_queue_size(MessageEventQueue* self) { return self->messageQueue.size + self->immediateQueue.size; }
static inline void message_event_queue_processing_lock(MessageEventQueue* self) { self->processingLock = true; }
static inline void message_event_queue_processing_unlock(MessageEventQueue* self) { self->processingLock = false; }

#define MESSAGE_SET_PARAM_BLOCK(msg, params)                    \
    memset(msg.paramBlock, 0, MESSAGE_PARAM_BLOCK_SIZE);        \
    memcpy(msg.paramBlock, ((u8*)((void*)&params)), MESSAGE_PARAM_BLOCK_SIZE)

#define MESSAGE_GET_PARAM_BLOCK(msg, dest) \
    memcpy(((u8*)((void*)&dest)), &msg.paramBlock, MESSAGE_PARAM_BLOCK_SIZE)

typedef void(*message_cb)(Component*, const Message);
typedef void(*system_message_cb)(AspectSystem*, Entity, const Message);

static inline void message_init(Message* self, MessageType type) {
    self->type = type;
}

inline void message_copy(const Message* source, Message* dest) {
    dest->type = source->type;
    memcpy(dest->paramBlock, source->paramBlock, MESSAGE_PARAM_BLOCK_SIZE);
}

#endif