#ifndef ASTEROIDS_PREFAB_H
#define ASTEROIDS_PREFAB_H

#include "types.h"
#include "debug.h"
#include "log.h"
#include "config.h"
#include "component.h"
#include "entitymanager.h"
#include <stdio.h>

void prefab_system_init(const char* prefabRoot);
void prefab_system_terminate();
void prefab_system_dump_names(FILE* outFile);

typedef struct prefab_t {
    Config* config;
    ComponentBatch components;
} Prefab;

typedef struct prefab_instantiation_t {
    char* name;
    Prefab* prefab;
    Vec2 position;
    f32 rotation;
} PrefabInstantiation;

void prefab_init(Prefab* self, char* filename);
void prefab_free(Prefab* self);
void prefab_free_void(void* self);
void prefab_reload(Prefab* self);
Prefab* prefab_get(const char* name);
Entity prefab_instantiate(Prefab* self, EntityManager* entityManager);
Entity prefab_instantiate_at(Prefab* self, EntityManager* entityManager, Vec2 position, f32 rotation);
Entity prefab_instantiate_with(PrefabInstantiation* instantiation, EntityManager* entityManager);

#endif