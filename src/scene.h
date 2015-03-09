#ifndef ASTEROIDS_SCENE_H
#define ASTEROIDS_SCENE_H

#include "types.h"
#include "debug.h"
#include "log.h"
#include "config.h"
#include "prefab.h"
#include "entitymanager.h"

typedef struct scene_t {
    Config* config;
    u32 instantiationCount;
    PrefabInstantiation* instantiations;
} Scene;

void scene_system_init(const char* sceneRoot);
void scene_system_terminate();

void scene_reload(Scene* self);
void scene_free(Scene* self);
void scene_free_void(void* self);
Scene* scene_get(const char* name);
void scene_instantiate(Scene* self, EntityManager* entityManager);

#endif