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
#define COMPONENT_LIST_SORT(type) COMPONENT_LIST_FUNC(type, sort)

typedef struct component_list_t {
    component_compare_f compareFunc;
    u32 count;
    u32 capacity;
    u32 componentSize;
} ComponentList;

#define COMPONENT_LIST_DECLARE_STRUCT(type)     \
    typedef struct component_list##type##_t {   \
        ComponentList super;                    \
        type* components;                       \
    } COMPONENT_LIST(type);

#define COMPONENT_LIST_INIT_PROTO(type) \
    void COMPONENT_LIST_INIT(type)(COMPONENT_LIST(type)* self, u32 capacity, component_compare_f compareFunc)

#define COMPONENT_LIST_FREE_PROTO(type) \
    void COMPONENT_LIST_FREE(type)(COMPONENT_LIST(type)* self)

#define COMPONENT_LIST_APPEND_PROTO(type) \
    void COMPONENT_LIST_APPEND(type)(COMPONENT_LIST(type)* self, type* component)

#define COMPONENT_LIST_GET_PROTO(type) \
    type* COMPONENT_LIST_GET(type)(COMPONENT_LIST(type)* self, Entity entity)

#define COMPONENT_LIST_GET_INDEX_PROTO(type) \
    i32 COMPONENT_LIST_GET_INDEX(type)(COMPONENT_LIST(type)* self, Entity entity)

#define COMOPONENT_LIST_GET_AT_PROTO(type) \
    type* COMPONENT_LIST_GET_AT(type)(COMPONENT_LIST(type)* self, u32 index)

#define COMPONENT_LIST_REMOVE_PROTO(type) \
    type* COMPONENT_LIST_REMOVE(type)(COMPONENT_LIST(type)* self, Entity entity)

#define COMPONNET_LIST_REMOVE_AT_PROTO(type) \
    type* COMPONENT_LIST_REMOVE_AT(type)(COMPONENT_LIST(type)* self, u32 index)

#define COMPONENT_LIST_SORT_PROTO(type) \
    void COMPONENT_LIST_SORT(type)(COMPONENT_LIST(type)* self)

#define COMPONENT_LIST_DECLARE(type)        \
    /* Declare the structure */             \
    COMPONENT_LIST_DECLARE_STRUCT(type);    \
                                            \
    /* Declare the functions */             \
    COMPONENT_LIST_INIT_PROTO(type);        \
    COMPONENT_LIST_FREE_PROTO(type);        \
    COMPONENT_LIST_APPEND_PROTO(type);      \
    COMPONENT_LIST_GET_PROTO(type);         \
    COMPONENT_LIST_GET_INDEX_PROTO(type);   \
    COMPONENT_LIST_GET_AT_PROTO(type);      \
    COMPONENT_LIST_REMOVE_PROTO(type);      \
    COMPONENT_LIST_REMOVE_AT_PROTO(type);   \
    COMPONENT_LIST_SORT_PROTO(type);

// Implementations
#define COMPONENT_LIST_INIT_IMPL(type)                      \
    COMPONENT_LIST_INIT_PROTO(type) {                       \
        self->super.compareFunc = compareFunc;              \
        self->super.count = 0;                              \
        self->super.capacity = capacity;                    \
        self->super.componentSize = sizeof(type);           \
        self->components = CALLOC(self->capacity, type);    \
    }

#define COMPONENT_LIST_FREE_IMPL(type)                      \
    COMPONENT_LIST_FREE_PROTO(type) {                       \
        free(self->components);                             \
    }

#define COMPONENT_LIST_APPEND_IMPL(type)                                                        \
    COMPONENT_LIST_APPEND_PROTO(type) {                                                         \
        ASSERT(self->super.count < self->super.capacity, "Maximum component count reached.");   \
                                                                                                \
        self->components[self->super.count] = component;                                        \
        ++self->super.count;                                                                    \
    }

#define COMPONENT_LIST_GET_IMPL(type)                           \
    COMPONENT_LIST_GET_PROTO(type) {                            \
       for (u32 i = 0; i < self->super.count; ++i) {            \
            Component c = self->components[i];                  \
            if (c.entity == entity) {                           \
                return c;                                       \
            }                                                   \
        }                                                       \
        return NULL;                                            \
    } 

#define COMPONENT_LIST_GET_INDEX_IMPL(type)                     \
    COMPONENT_LIST_GET_INDEX_PROTO(type) {                      \
        for (u32 i = 0; i < self->super.count; ++i) {           \
            Component c = self->components[i];                  \
            if (c.entity == entity) {                           \
                return (i32)i;                                  \
            }                                                   \
        }                                                       \
                                                                \
        return -1;                                              \
    }

#define COMPONENT_LIST_GET_AT_IMPL(type)                            \
    COMPONENT_LIST_GET_AT_PROTO(type) {                             \
        ASSERT(index < self->super.count, "Index out of range.");   \
        return &self->components[index];                            \
    }

#define COMPONENT_LIST_REMOVE_IMPL(type)                        \
    COMPONENT_LIST_REMOVE_PROTO(type) {                         \
        i32 index = component_list_get_index(self, entity);     \
        if (index >= 0) {                                       \
            return component_list_remove_at(self, (u32)index);  \
        } else {                                                \
            return NULL;                                        \
        }                                                       \
    }

#define COMPONENT_LIST_REMOVE_AT_IMPL(type)                                     \
    COMPONENT_LIST_REMOVE_AT_PROTO(type) {                                      \
        Component* result = &self->components[index];                           \
                                                                                \
        if (self->super.compareFunc == NULL) {                                  \
            self->components[index] = self->components[self->super.count - 1];  \
        } else {                                                                \
            for (u32 i = index; i < self->super.count - 1; ++i) {               \
                self->components[i] = self->components[i + 1];                  \
            }                                                                   \
        }                                                                       \
                                                                                \
        --self->super.count;                                                    \
        return result;                                                          \
    }

#define COMPONENT_LIST_SORT_IMPL(type)                          \
    COMPONENT_LIST_SORT_PROTO(type) {                           \
        for (u32 i = 1; i < self->super.count; ++i) {           \
            u32 j = i;                                          \
            Component* c1 = &self->components[j - 1];           \
            Component* c2 = &self->components[j];               \
            while (j > 0 && self->compareFunc(c1, c2) > 0) {    \
                Component* temp = self->components[j - 1];      \
                self->components[j - 1] = self->components[j];  \
                self->components[j] = temp;                     \
                --j;                                            \
            }                                                   \
        }                                                       \
    }

#define COMPONENT_LIST_DEFINE(type) \
    COMPONENT_LIST_INIT_IMPL(type)         \
    COMPONENT_LIST_FREE_IMPL(type)         \
    COMPONENT_LIST_APPEND_IMPL(type)       \
    COMPONENT_LIST_GET_IMPL(type)          \
    COMPONENT_LIST_GET_INDEX_IMPL(type)    \
    COMPONENT_LIST_GET_AT_IMPL(type)       \
    COMPONENT_LIST_REMOVE_IMPL(type)       \
    COMPONENT_LIST_REMOVE_AT_IMPL(type)    \
    COMPONENT_LIST_SORT_IMPL(type)


#endif