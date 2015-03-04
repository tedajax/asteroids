#include "scene.h"
#include "directory.h"

ConfigSystem sceneConfigs;
Hashtable sceneTable;

void scene_load(const char* filename) {
    if (config_system_load(&sceneConfigs, filename)) {
        Config* cfg = config_system_get(&sceneConfigs, filename);
        Scene* newScene = CALLOC(1, Scene);
        newScene->config = cfg;
        scene_reload(newScene);
        hashtable_insert(&sceneTable, filename, (void*)newScene);
    }
}

void scene_system_init(const char* sceneRoot) {
    config_system_init(&sceneConfigs, (char*)sceneRoot, "scene");
    hashtable_init(&sceneTable, 32, scene_free_void);

    Directory* sceneDir = directory_open(sceneRoot);
    FileDescriptor* currentFile;
    while ((currentFile = directory_next(sceneDir))) {
        size_t len = strlen(currentFile->filename);
        if (len > 6 && strcmp(&currentFile->filename[len - 6], ".scene") == 0) {
            scene_load(currentFile->filename);
        }
    }
    directory_close(sceneDir);
}

void scene_system_terminate() {
    config_system_terminate(&sceneConfigs);
    hashtable_free_contents(&sceneTable);
}

void scene_reload(Scene* self) {
    u32 count = (u32)CONFIG_GET(array_count)(self->config, CONFIG_DEFAULT_SECTION, "instantiations");
    if (!count) { return; }

    self->instantiationCount = count;
    self->instantiations = CALLOC(self->instantiationCount, PrefabInstantiation);

    for (u32 i = 0; i < self->instantiationCount; ++i) {
        char* section = CONFIG_GET_AT(string)(self->config, CONFIG_DEFAULT_SECTION, "instantiations", i);
        char* prefabName = CONFIG_GET(string)(self->config, section, "prefab");
        Prefab* prefab = prefab_get(prefabName);
        ASSERT(prefab, "Unable to find prefab.");
        Vec2 position = CONFIG_TRY_GET(Vec2)(self->config, section, "position", vec2_zero());
        f32 rotation = CONFIG_TRY_GET(float)(self->config, section, "rotation", 0.f);
        char* instantiationName = CONFIG_TRY_GET(string)(self->config, section, "name", NULL);

        PrefabInstantiation* instantiation = &self->instantiations[i];
        instantiation->name = instantiationName;
        instantiation->prefab = prefab;
        instantiation->position = position;
        instantiation->rotation = rotation;
    }
}

void scene_free(Scene* self) {
    if (self->instantiationCount) {
        free(self->instantiations);
    }
    free(self);
}

void scene_free_void(void* self) {
    scene_free((Scene*)self);
}

Scene* scene_get(const char* name) {
    return (Scene*)hashtable_get(&sceneTable, name);
}

void scene_instantiate(Scene* self, EntityManager* entityManager) {
    log_info_format("Scene", "Loading scene: %s", self->config->path);
    for (u32 i = 0; i < self->instantiationCount; ++i) {
        prefab_instantiate_with(&self->instantiations[i], entityManager);
    }
    log_info_format("Scene", "Done loading scene: %s", self->config->path);
}
