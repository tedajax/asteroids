#include "entitymanager.h"
#include "collidercomponent.h"
#include "spritecomponent.h"
#include "collisionsystem.h"
#include "aspectsystem.h"
#include "prefab.h"

void entities_internal_remove_entity(EntityManager* self, Entity entity, bool isShutdown);
void entities_internal_send_message(EntityManager* self, TargetedMessage message);

POOL_IMPLEMENTATION(Entity);

//////////////////
// Entity Queue //
//////////////////

void entity_queue_init(EntityQueue* self) {
    self->length = 0;
    self->head = 0;
    self->tail = 0;
}

void entity_queue_push(EntityQueue* self, Entity entity) {
    ASSERT(self->length < MAX_ENTITIES, "Reached maximum size of entity queue.");
    self->entities[self->tail] = entity;
    ++self->tail;
    ++self->length;
}

Entity entity_queue_pop(EntityQueue* self) {
    Entity result = self->entities[self->head];
    ++self->head;
    if (self->head == self->tail) {
        self->head = 0;
        self->tail = 0;
    }
    --self->length;
    return result;
}

/////////////////
// Entity List //
/////////////////

void entity_list_init(EntityList* self, u32 capacity) {
    self->list = (Entity *)calloc(capacity, sizeof(Entity));
    self->capacity = capacity;
    self->size = 0;
}

void entity_list_resize(EntityList* self, u32 capacity) {
    self->capacity = capacity;
    self->list = (Entity *)realloc(self->list, capacity * sizeof(Entity));

    if (capacity < self->size) {
        self->size = capacity;
    }
}

void entity_list_free(EntityList* self) {
    free(self->list);
}

/////////////////
// Entity Dict //
/////////////////
u64 _djb2(const char *key) {
    u64 hash = 5381;
    i32 c;
    while ((c = *key++))
    {
        hash = ((hash << 5) + hash) + c; //hash * 33 + c
    }
    return hash;
}

void entity_dict_init(EntityDict* self, u32 bucketCount) {
    self->bucketCount = bucketCount;
    self->buckets = CALLOC(self->bucketCount, EntityDictNode*);
}

void entity_dict_cleanup(EntityDict* self) {
    entity_dict_clear(self);
    free(self->buckets);
}

void entity_dict_clear(EntityDict* self) {
    for (u32 i = 0; i < self->bucketCount; ++i) {
        EntityDictNode* node = self->buckets[i];
        while (node) {
            EntityDictNode* next = node->next;
            free(node);
            node = next;
        }
        self->buckets[i] = NULL;
    }
}

EntityDictNode* entity_dict_remove_node(EntityDict* self, EntityDictNode* node) {
    u32 index = node->keyHash % self->bucketCount;
    EntityDictNode* current = self->buckets[index];

    ASSERT(current, "Attempt to remove node that is not in this dictionary.");

    while (current && current != node) {
        current = current->next;
    }

    ASSERT(current, "Attempt to remove node that is not in this dictionary.");

    if (current->prev) {
        current->prev->next = current->next;
    }
    if (current->next) {
        current->next->prev = current->prev;
    }

    current->entity = 0;
    current->keyHash = 0;

    return current;
}

void entity_dict_insert(EntityDict* self, const char* name, Entity entity) {
    u64 hash = _djb2(name);
    u32 index = hash % self->bucketCount;

    EntityDictNode* node = self->buckets[index];

    if (!node) {
        node = CALLOC(1, EntityDictNode);
        node->keyHash = hash;
        node->entity = entity;
        node->prev = NULL;
        node->next = NULL;
        self->buckets[index] = node;
        return;
    }

    ASSERT(node->keyHash != hash, "Key already exists in EntityDict.");

    while (node->next) {
        ASSERT(node->keyHash != hash, "Key already exists in EntityDict.");
        node = node->next;
    }

    EntityDictNode* newNode = CALLOC(1, EntityDictNode);
    newNode->keyHash = hash;
    newNode->entity = entity;
    newNode->prev = node;
    newNode->next = NULL;
    node->next = newNode;

    printf("%p %p %p\n", node->next, newNode, newNode->next);
}

Entity entity_dict_get(EntityDict* self, const char* name) {
    u64 hash = _djb2(name);
    u32 index = hash % self->bucketCount;

    EntityDictNode* node = self->buckets[index];

    while (node) {
        if (node->keyHash == hash) {
            return node->entity;
        }
        node = node->next;
    }

    return 0;
}

EntityDictIter entity_dict_iter(EntityDict* self) {
    EntityDictIter iter;
    iter.dictionary = self;
    iter.currentBucket = 0;
    iter.currentNode = self->buckets[0];

    while (!iter.currentNode) {
        ++iter.currentBucket;
        if (iter.currentBucket >= self->bucketCount) {
            break;
        }
        iter.currentNode = self->buckets[iter.currentBucket];
    }

    return iter;
}

EntityDictNode* entity_dict_next(EntityDictIter* self) {
    if (self->currentBucket >= self->dictionary->bucketCount) {
        return NULL;
    }
    
    EntityDictNode* result = self->currentNode;

    if (self->currentNode->next) {
        self->currentNode = self->currentNode->next;
    } else {
        do {
            ++self->currentBucket;
            self->currentNode = self->dictionary->buckets[self->currentBucket];
        } while (!self->currentNode && self->currentBucket < self->dictionary->bucketCount);
    }

    if (self->currentBucket >= self->dictionary->bucketCount) { self->currentNode = NULL;  }

    return result;
}

////////////////////
// Entity Manager //
////////////////////

EntityManager* entity_manager_new() {
    EntityManager* self = (EntityManager*)calloc(1, sizeof(EntityManager));

    POOL_INIT(Entity)(&self->entities, MAX_ENTITIES, 0);
    /*for (u32 i = 0; i < COMPONENT_LAST; ++i) {
        component_list_init(&self->componentsMap[i], component_entity_compare);
    }*/
    self->lowestEId = 1;

    for (u32 c = 0; c < COMPONENT_LAST; ++c) {
        self->systems[c] = NULL;
    }

    entity_queue_init(&self->removeQueue);
    message_event_queue_init(&self->eventQueue);
    messaging_system_init(&self->messagingSystem);
    entity_dict_init(&self->namedEntities, 8);

    return self;
}

void entity_manager_free(EntityManager* self) {
    entities_remove_all_entities(self);
    messaging_system_terminate(&self->messagingSystem);
    for (ComponentType t = COMPONENT_INVALID + 1; t < COMPONENT_LAST; ++t) {
        component_list_free(&self->componentsMap[t]);
    }
    entity_dict_cleanup(&self->namedEntities);
    POOL_FREE(Entity)(&self->entities);
    free(self);
}

void entity_manager_register_system(EntityManager* self, AspectSystem* system, u32 capacity) {
    ASSERT(self && system, "");
    
    ComponentType type = system->systemType;

    ASSERT(self->systems[type] == NULL, "System of that type already registered!");

    self->systems[type] = system;
    component_list_init(&self->componentsMap[type], capacity, component_entity_compare);
}

void entity_manager_set_system_compare_function(EntityManager* self, AspectSystem* system, component_compare_f compareFunc) {
    ComponentType type = system->systemType;
    self->componentsMap[type].compareFunc = compareFunc;
}

i32 entities_gen_entity_id(EntityManager* self) {
    ASSERT(self->lowestEId < 0xFFFFFFFF, "How the fuck?");
    return self->lowestEId++;
}

Entity entities_create_entity(EntityManager* self) {
    Entity entity = entities_gen_entity_id(self);
    POOL_INSERT(Entity)(&self->entities, (entity));

    Message msg;
    msg.type = MESSAGE_ENTITY_ADDED;

    MessageOnAddedParams params;
    params.addedEntity = entity;
    MESSAGE_SET_PARAM_BLOCK(msg, params);

    SubscriberList* list = &self->messagingSystem.subscriberLists[msg.type];
    for (u32 i = 0; i < list->count; ++i) {
        for (u32 t = 0; t < COMPONENT_LAST; ++t) {
            if (self->systems[t]) {
                aspect_system_send_message((AspectSystem*)self->systems[t],
                    list->subscribers[i],
                    msg);
            }
        }
    }

    return entity;
}

Entity entities_instantiate_prefab(EntityManager* self, Prefab* prefab) {
    Entity entity = entities_create_entity(self);

    for (u32 i = 0; i < prefab->components.count; ++i) {
        Component* source = prefab->components.components[i];
        
        // Copy into newly allocated block that will actually go into entity manager
        Component* component = (Component*)malloc((size_t)source->size);
        component_copy(source, component);

        // TODO: generalize these special cases somewhat?
        if (component->type == COMPONENT_COLLIDER) {
            ColliderComponent* collider = (ColliderComponent*)component;
            collider->collider.entity = entity;
        }

        // Set the entity
        component->entity = entity;

        entities_add_component(self, component);
    }

    return entity;
}

Entity entities_instantiate_prefab_at(EntityManager* self, Prefab* prefab, Vec2 position, f32 rotation) {
    Entity entity = entities_create_entity(self);

    for (u32 i = 0; i < prefab->components.count; ++i) {
        Component* source = prefab->components.components[i];

        // Copy into newly allocated block that will actually go into entity manager
        Component* component = (Component*)malloc((size_t)source->size);
        component_copy(source, component);

        // TODO: generalize these special cases somewhat?
        if (component->type == COMPONENT_TRANSFORM) {
            TransformComponent* tx = (TransformComponent*)component;
            tx->position = position;
            tx->rotation = rotation;
        } else if (component->type == COMPONENT_COLLIDER) {
            ColliderComponent* collider = (ColliderComponent*)component;
            collider->collider.entity = entity;
        }

        // Set the entity
        component->entity = entity;

        entities_add_component(self, component);
    }

    return entity;
}

Component* entities_add_component(EntityManager* self, Component* component) {
    ASSERT(component->type > COMPONENT_INVALID && component->type < COMPONENT_LAST,
        "Invalid component, did you remember to set the component type in the component constructor?");

    component_list_append(&self->componentsMap[component->type], component);

    return component;
}

Component* entities_get_component(EntityManager* self, ComponentType type, Entity entity) {
    profiler_tick("get_component");
    Component* result = component_list_get(&self->componentsMap[type], entity);
    profiler_tock_context("get_component", component_type_get_name(type));
    return result;
}

ComponentList* entities_get_all_components(EntityManager* self, ComponentType type) {
    return &self->componentsMap[type];
}

bool entities_has_component(EntityManager* self, ComponentType type, Entity entity) {
    return entities_get_component(self, type, entity) != NULL;
}

void entities_internal_remove_entity(EntityManager* self, Entity entity, bool isShutDown) {
    // Send on_remove message
    // but only if this is an individual remove and not all entities being removed
    if (!isShutDown) {
        Message msg;
        msg.type = MESSAGE_ENTITY_REMOVED;

        MessageOnRemovedParams params;
        params.removedEntity = entity;
        MESSAGE_SET_PARAM_BLOCK(msg, params);

        TargetedMessage targetedMsg = {
            entity,
            msg
        };

        entities_internal_send_message(self, targetedMsg);

        EntityDictIter iter = entity_dict_iter(&self->namedEntities);
        EntityDictNode* node;
        while ((node = entity_dict_next(&iter))) {
            if (node->entity == entity) {
                EntityDictNode* removed = entity_dict_remove_node(iter.dictionary, node);
                if (removed->prev) {
                    free(removed);
                }
                break;
            }
        }
    }

    // Actually remove the components
    for (u32 t = COMPONENT_INVALID + 1; t < COMPONENT_LAST; ++t) {
        Component* toRemove = component_list_remove(&self->componentsMap[t], entity);
        if (toRemove != NULL) {
            component_free(toRemove);
        }
    }

    // Free up the entity from the pool
    for (u32 i = 0; i < self->entities.capacity; ++i) {
        Entity* e = POOL_GET(Entity)(&self->entities, i);
        if (e && *e == entity) {
            POOL_REMOVE_AT(Entity)(&self->entities, i);
            break;
        }
    }
}

void entities_remove_entity(EntityManager* self, Entity entity) {
    entity_queue_push(&self->removeQueue, entity);
}

void entities_remove_all_entities(EntityManager* self) {
    u32 len = self->entities.capacity;
    for (u32 i = 0; i < len; ++i) {
        Entity e = *POOL_GET(Entity)(&self->entities, i);
        if (e) {
            entities_internal_remove_entity(self, e, true);
        }
    }
    entity_dict_clear(&self->namedEntities);
}

void entities_get_all_of(EntityManager* self, ComponentType type, EntityList* dest) {
    /* 
        This function is no longer necessary with the new architecture but it's worth
        recreating while we try and get everything running with new stuff.
    */

    ComponentList* components = &self->componentsMap[type];
    dest->size = 0;
    for (u32 i = 0; i < components->count; ++i) {
        ASSERT(dest->size < dest->capacity, "Reached maximum size of entity list.  Make it bigger!");
        dest->list[i] = components->components[i]->entity;
        ++dest->size;
    }
}

void entities_internal_send_message(EntityManager* self, TargetedMessage message) {
    for (u32 type = COMPONENT_INVALID + 1; type < COMPONENT_LAST; ++type) {
        // Send message to all subscribers of this type of message
        // IMPORTANT NOTE: If the entity in the subscriber list is the same
        // as the targeted entity then the message will NOT be fired a second time
        // to avoid the weird kinds of things that could happen if an entity received
        // the same message twice.
        MessageType mtype = message.message.type;
        SubscriberList* subList = &self->messagingSystem.subscriberLists[mtype];
        for (u32 i = 0; i < subList->count; ++i) {
            Entity entity = subList->subscribers[i];
            if (entity == message.target) { continue; }

            if (self->systems[type]) {
                aspect_system_send_message((AspectSystem*)self->systems[type],
                    entity,
                    message.message);
            }
        }

        if (!entities_has_component(self, type, message.target)) {
            continue;
        }

        if (self->systems[type]) {
            aspect_system_send_message((AspectSystem*)self->systems[type],
                message.target,
                message.message);
        }
    }
}

void entities_send_message(EntityManager* self, Entity entity, Message message) {
    message_event_queue_push(&self->eventQueue, entity, message);
}

void entities_send_message_deferred(EntityManager* self, Entity entity, Message message) {
    message_event_queue_push_deferred(&self->eventQueue, entity, message);
}

void entities_subscribe(EntityManager* self, MessageType messageType, Entity subscriber) {
    messaging_system_add_subscriber(&self->messagingSystem, messageType, subscriber);
}

void entities_update(EntityManager* self) {
    const i32 maxMessages = 0xF;
    i32 messageCount = 0;
    message_event_queue_processing_lock(&self->eventQueue);
    while (message_event_queue_size(&self->eventQueue) > 0 && messageCount < maxMessages) {
        TargetedMessage message = message_event_queue_pop(&self->eventQueue);
        entities_internal_send_message(self, message);
        ++messageCount;
    }
    message_event_queue_processing_unlock(&self->eventQueue);

    while (self->removeQueue.length > 0) {
        entities_internal_remove_entity(self, entity_queue_pop(&self->removeQueue), false);
    }
}

void entities_add_named_entity(EntityManager* self, const char* name, Entity entity) {
    log_info_format("Named Entity", "%s: %d", name, entity);
    entity_dict_insert(&self->namedEntities, name, entity);
}

Entity entities_get_named_entity(EntityManager* self, const char* name) {
    return entity_dict_get(&self->namedEntities, name);
}