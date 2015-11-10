#ifndef ASTEROIDS_COMPONENT_LIST_H
#define ASTEROIDS_COMPONENT_LIST_H

#include "core.h"
#include "component.h"

typedef int(*component_compare_f)(Component*, Component*);

#define COMPONENT_LIST(type) ComponentList##type

#define COMPONENT_LIST_FUNC(type, func) component_list_##type##_##func

#define COMPONENT_LIST_INIT(type) COMPONENT_LIST_FUNC(type, init)
#define COMPONENT_LIST_FREE(type) COMPONENT_LIST_FUNC(type, free)
#define COMPONENT_LIST_APPEND(type) COMPONENT_LIST_FUNC(type, append)
#define COMPONENT_LIST_GET(type) COMPONENT_LIST_FUNC(type, get)
#define COMPONENT_LIST_GET_INDEX(type) COMPONENT_LIST_FUNC(type, get_index)
#define COMPONENT_LIST_GET_AT(type) COMPONENT_LIST_FUNC(type, get_at)
#define COMPONENT_LIST_REMOVE(type) COMPONENT_LIST_FUNC(type, remove)
#define COMPONENT_LIST_REMOVE_AT(type) COMPONENT_LIST_FUNC(type, remove_at)

#define COMPONENT_LIST_DEFINE_STRUCT(type)  \
    typedef struct component_list_t {       \
        component_compare_f compareFunc;    \
        u32 count;                          \
        u32 capacity;                       \
        type* components;                   \
    } COMPONENT_LIST(type);

#define COMPONENT_LIST_DECLARE_INIT(type) \
    void COMPONENT_LIST_INIT(type)(COMPONENT_LIST(type)* self, u32 capacity, component_compare_f compareFunc)

#define COMPONENT_LIST_DECLARE_FREE(type) \
    void COMPONENT_LIST_FREE(type)(COMPONENT_LIST(type)* self)

#define COMPONENT_LIST_DECLARE_APPEND(type) \
    void COMPONENT_LIST_APPEND(type)(COMPONENT_LIST(type)* self, type* component)

#define COMPONENT_LIST_DECLARE_GET(type) \
    type* COMPONENT_LIST_GET(type)(COMPONENT_LIST(type)* self, Entity entity)

#define COMPONENT_LIST_DECLARE_GET_INDEX(type) \
    i32 COMPONENT_LIST_GET_INDEX(type)(COMPONENT_LIST(type)* self, Entity entity)

#define COMOPONENT_LIST_DECLARE_GET_AT(type) \
    type* COMPONENT_LIST_GET_AT(COMPONENT_LIST(type)* self, u32 index)

void component_list_init(ComponentList* self, u32 capacity, component_compare_f compareFunc);
void component_list_free(ComponentList* self);
void component_list_append(ComponentList* self, Component* component);
Component* component_list_get(ComponentList* self, Entity entity);
i32 component_list_get_index(ComponentList* self, Entity entity);
Component* component_list_get_at(ComponentList* self, u32 index);
Component* component_list_remove(ComponentList* self, Entity entity);
Component* component_list_remove_at(ComponentList* self, u32 index);

void component_list_insertion_sort(ComponentList* self);

#endif