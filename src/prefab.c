#include "prefab.h"
#include "entitymanager.h"
#include "game.h"
#include "directory.h"

ConfigSystem prefabConfigs;
Hashtable prefabTable;

void prefab_load(const char* filename) {
    if (config_system_load(&prefabConfigs, filename)) {
        Config* cfg = config_system_get(&prefabConfigs, filename);
        Prefab* newPrefab = CALLOC(1, Prefab);
        newPrefab->config = cfg;
        prefab_reload(newPrefab);
        hashtable_insert(&prefabTable, filename, (void*)newPrefab);
    }
}

void prefab_system_init(const char* prefabRoot) {
    config_system_init(&prefabConfigs, (char*)prefabRoot, "prefab");
    hashtable_init(&prefabTable, 32, prefab_free_void);

    Directory* prefabDir = directory_open(prefabRoot);
    FileDescriptor* currentFile;
    while ((currentFile = directory_next(prefabDir))) {
        size_t len = strlen(currentFile->filename);
        // Only add the file if it has the '.prefab' extensions
        if (len > 7 && strcmp(&currentFile->filename[len - 7], ".prefab") == 0) {
            prefab_load(currentFile->filename);
        }
    }
    directory_close(prefabDir);
}

void prefab_system_terminate() {
    config_system_terminate(&prefabConfigs);
    hashtable_free_contents(&prefabTable);
}

void prefab_system_dump_names(FILE* outFile) {
    HashtableIter iter = hashtable_get_iter(&prefabConfigs.configTable);
    Config* cfg;

    fprintf(outFile, "\nPrefab system dump:\n");
    do {
        cfg = hashtable_next(&iter);
        if (!cfg) { break; }
        fprintf(outFile, "\t%s\n", cfg->path);
    } while (cfg);
    fprintf(outFile, "\n");
}

Prefab* prefab_new(char* filename) {
    Prefab* self = CALLOC(1, Prefab);
    prefab_init(self, filename);
    return self;
}

void prefab_init(Prefab* self, char* filename) {
    self->config = config_system_get(&prefabConfigs, filename);
    component_batch_zero(&self->components);
}

void prefab_free(Prefab* self) {
    component_batch_free(&self->components);
    free(self);
}

void prefab_free_void(void* self) {
    prefab_free((Prefab*)self);
}

void prefab_reload(Prefab* self) {
    u32 dependencyCount = (u32)config_get_array_count(self->config, CONFIG_DEFAULT_SECTION, "dependencies");
    // So many potential issues here, should clean this up
    for (u32 i = 0; i < dependencyCount; ++i) {
        char* name = CONFIG_GET_AT(string)(self->config, CONFIG_DEFAULT_SECTION, "dependencies", i);
        prefab_load(name);
    }

    u32 componentCount = (u32)config_get_array_count(self->config, CONFIG_DEFAULT_SECTION, "components");
    component_batch_init(&self->components, componentCount);

    for (u32 i = 0; i < componentCount; ++i) {
        char* name = CONFIG_GET_AT(string)(self->config, CONFIG_DEFAULT_SECTION, "components", i);
        Component* component = component_deserialize(self->config, name);
        component_batch_add(&self->components, component);
    }
}

Prefab* prefab_get(const char* name) {
    Prefab* result = (Prefab*)hashtable_get(&prefabTable, name);
    if (!result) {
        log_error_format("Prefab", "Failed to load prefab with name \'%s\'!", name);
    }
    return result;
}

Entity prefab_instantiate(Prefab* self, EntityManager* entityManager) {
    return entities_instantiate_prefab(entityManager, self);
}

Entity prefab_instantiate_at(Prefab* self, EntityManager* entityManager, Vec2 position, f32 rotation) {
    return entities_instantiate_prefab_at(entityManager, self, position, rotation);
}

Entity prefab_instantiate_with(PrefabInstantiation* instantiation, EntityManager* entityManager) {
    return prefab_instantiate_at(instantiation->prefab, entityManager, instantiation->position, instantiation->rotation);
}