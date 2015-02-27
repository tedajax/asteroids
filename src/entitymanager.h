#ifndef RUNNER_ENTITY_MANAGER_H
#define RUNNER_ENTITY_MANAGER_H

#include "core.h"

#include "pool.h"
#include "entity.h"
#include "component.h"
#include "componentlist.h"
#include "messaging.h"

// Forward declarations
typedef struct collision_system_t CollisionSystem;
typedef struct aspect_system_t AspectSystem;
typedef struct prefab_t Prefab;

typedef struct entity_queue_t {
    Entity entities[MAX_ENTITIES];
    u32 length;
    u32 head;
    u32 tail;
} EntityQueue;

void entity_queue_init(EntityQueue* self);
void entity_queue_push(EntityQueue* self, Entity entity);
Entity entity_queue_pop(EntityQueue* self);

typedef struct entity_id_list_t {
    Entity* list;
    u32 size;
    u32 capacity;
} EntityList;

void entity_list_init(EntityList* self, u32 capacity);
void entity_list_resize(EntityList* self, u32 capacity);
void entity_list_free(EntityList* self);

#define ENTITY_MANAGER_MESSAGE_QUEUE_CAPACITY 128

POOL_REGISTER(Entity);

typedef struct entity_manager_t {
    POOL(Entity) entities;
    AspectSystem* systems[COMPONENT_LAST];
    ComponentList componentsMap[COMPONENT_LAST];
    i32 lowestEId;
    EntityQueue removeQueue;
    MessageEventQueue eventQueue;
} EntityManager;

EntityManager* entity_manager_new();
void entity_manager_free(EntityManager* self);
void entity_manager_register_system(EntityManager* self, AspectSystem* system, u32 capacity);
void entity_manager_set_system_compare_function(EntityManager* self, AspectSystem* system, component_compare_f compareFunc);
i32 entities_gen_entity_id(EntityManager* self);
Entity entities_create_entity(EntityManager* self);
Entity entities_instantiate_prefab(EntityManager* self, Prefab* prefab);
Entity entities_instantiate_prefab_at(EntityManager* self, Prefab* prefab, Vec2 position, f32 rotation);
Component* entities_add_component(EntityManager* self, Component* component);
Component* entities_get_component(EntityManager* self, ComponentType type, Entity entity);
ComponentList* entities_get_all_components(EntityManager* self, ComponentType type);
bool entities_has_component(EntityManager* self, ComponentType type, Entity entity);
void entities_remove_entity(EntityManager* self, Entity entity);
void entities_remove_all_entities(EntityManager* self);
//void entities_get_all_of(EntityManager* self, ComponentType type, EntityList* dest);
void entities_send_message(EntityManager* self, Entity entity, Message message);
void entities_send_message_deferred(EntityManager* self, Entity entity, Message message);

// Should be called AFTER update AND render calls have been made!
void entities_update(EntityManager* self);

#define REGISTER_SYSTEM(self, system, capacity) \
    entity_manager_register_system(self, (AspectSystem*)system, capacity)

#define SET_SYSTEM_COMPARISON(entitymanager, system, compare) \
    entity_manager_set_system_compare_function(entitymanager, (AspectSystem*)system, compare)

#endif