#include "entitymanager.h"
#include "collidercomponent.h"
#include "collisionsystem.h"
#include "aspectsystem.h"

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

EntityManager* entity_manager_new() {
    EntityManager* self = (EntityManager*)calloc(1, sizeof(EntityManager));

    vector_init(&self->entities, 256, NULL);
    for (u32 i = 0; i < COMPONENT_LAST; ++i) {
        dict_init(&self->componentsMap[i], 64, component_free_void);
    }
    self->lowestEId = 1;


    for (u32 c = 0; c < COMPONENT_LAST; ++c) {
        for (u32 i = 0; i < ENTITY_MANAGER_MAX_SYSTEM_COUNT; ++i) {
            self->systems[c][i] = NULL;
        }
        self->systemCounts[c] = 0;
    }

    return self;
}

void entity_manager_free(EntityManager* self) {
    entities_remove_all_entities(self);
    for (u32 i = 0; i < COMPONENT_LAST; ++i) {
        dict_clear(&self->componentsMap[i]);
    }
    free(self->entities.data);
    free(self);
}

void entity_manager_register_system(EntityManager* self, AspectSystem* system) {
    ASSERT(self && system, "");
    
    ComponentType type = system->systemType;

    ASSERT(self->systemCounts[type] < ENTITY_MANAGER_MAX_SYSTEM_COUNT - 1, "");

    u32 count = self->systemCounts[type];
    self->systems[type][count] = system;
    ++self->systemCounts[type];
}

u32 entities_gen_entity_id(EntityManager* self) {
    ASSERT(self->lowestEId < 0xFFFFFFFF, "How the fuck?");
    return self->lowestEId++;
}

Entity* entities_create_entity(EntityManager* self) {
    u32 id = entities_gen_entity_id(self);
    Entity* entity = entity_new(id);
    vector_add(&self->entities, entity);
    return entity;
}

void entities_add_component(EntityManager* self, Component* component, Entity* entity) {
    ASSERT(component->type > COMPONENT_INVALID && component->type < COMPONENT_LAST,
        "Invalid component, did you remember to set the component type in the component constructor?");

    Dictionary components = self->componentsMap[component->type];

    dict_set(&components, entity->id, component);
}

Component* entities_get_component(EntityManager* self, ComponentType type, Entity* entity) {
    return dict_get(&self->componentsMap[type], entity->id, 0);
}

DictListNode* entities_get_components(EntityManager* self, ComponentType type, Entity* entity) {
    return dict_get_all(&self->componentsMap[type], entity->id);
}

void entities_remove_entity(EntityManager* self, Entity* entity) {
    for (u32 t = 0; t < COMPONENT_LAST; ++t) {
        Dictionary components = self->componentsMap[t];
        DictListNode* clist = (DictListNode*)dict_remove(&components, entity->id);
        
        while (clist != NULL) {
            // This seems pretty gross but I haven't come up with anything better yet
            // probably should fire events when an entity is removed!
            if (t == COMPONENT_COLLIDER) {
                ColliderComponent* collider = (ColliderComponent*)clist->element;
                for (int i = 0; i < ENTITY_MANAGER_MAX_SYSTEM_COUNT; ++i) {
                    if (self->systems[t][i] != NULL) {
                        collision_system_remove_collider((CollisionSystem*)self->systems[t][i], collider);
                    }
                }                
            }

            component_free_void(clist->element);
            DictListNode* prev = clist;
            clist = clist->next;
            free(prev);
        }
    }

    for (u32 i = 0; i < self->entities.size; ++i) {
        Entity* e = (Entity *)vector_index(&self->entities, i);
        if (e->id == entity->id) {
            Entity* removed = (Entity*)vector_removeAt(&self->entities, i);
            free(removed);
            break;
        }
    }
}

void entities_remove_all_entities(EntityManager* self) {
    u32 len = self->entities.size;
    for (u32 i = 0; i < len; ++i) {
        entities_remove_entity(self, (Entity*)vector_index(&self->entities, 0));
    }
}

void entities_get_all_of(EntityManager* self, ComponentType type, EntityList* dest) {
    Dictionary components = self->componentsMap[type];

    dest->size = 0;

    u32 index = 0;

    for (u32 b = 0; b < components.bucketCount; ++b) {
        DictionaryNode* node = &components.buckets[b];
        while (node != NULL && node->key != DICT_INVALID_KEY) {
            if (node->list != NULL) {
                if (dest->size >= dest->capacity) {
                    entity_list_resize(dest, dest->capacity * 2);
                }
                dest->list[index++].id = node->key;
                ++dest->size;
            }
            node = node->next;
        }
    }
}

void entities_send_message(EntityManager* self, Entity* entity, Message message) {
    for (u32 type = COMPONENT_INVALID + 1; type < COMPONENT_LAST; ++type) {
        DictListNode* node = entities_get_components(self, type, entity);

        while (node) {
            if (node->element) {
                component_send_message((Component*)node->element, message);
            }
            node = node->next;
        }
    }
}

u32 entities_entity_count(EntityManager* self) {
    return self->entities.size;
}