#include "config.h"
#include "snprintf.h"

void config_system_init(ConfigSystem* self, char* rootDir, char* fileDescriptor) {
    self->rootDir = rootDir;
    self->fileDescriptor = fileDescriptor;
    hashtable_init(&self->configTable, 32, config_type_free_void);
}

void config_system_terminate(ConfigSystem* self) {
    self->rootDir = NULL;
    hashtable_free_contents(&self->configTable);
}

void config_type_free(Config* self) {
    hashtable_free_contents(&self->typeConfigs);
    ini_free(&self->data);
    free(self->path);
    free(self);
}

void config_type_free_void(void* pself) {
    config_type_free((Config*)pself);
}

void type_config_free(TypeConfig* self) {
    CLEANUP(self->type, self);
    free(self->tableName);
    free(self);
}

void type_config_free_void(void* self) {
    type_config_free((TypeConfig*)self);
}

bool config_type_update_mtime(Config* self) {
    time_t mtime = config_get_mtime(self->path);
    
    if (mtime > self->lastMTime) {
        // TODO: reload config, propagate changes to relevant config types
        self->lastMTime = mtime;
        return true;
    }

    return false;
}

void config_type_reload(Config* self) {
    ini_free(&self->data);
    ini_init(&self->data);
    ini_load(&self->data, self->path);

    TypeConfig* typeConfigs[128];
    u32 count = (u32)hashtable_get_all(&self->typeConfigs, (void**)typeConfigs, 128);

    for (u32 i = 0; i < count; ++i) {
        TypeConfig* typeConfig = typeConfigs[i];
        DESERIALIZE(typeConfig->type, typeConfig, self, typeConfig->tableName);
    }
}

void config_init() {
    config_system_init(&defaultConfigs, "assets/data/", "config file");

    
    REGISTER_CONFIG_TYPE_FUNCTIONS(TYPE_CONFIG_COLLIDER, collider_config_deserialize, NULL);
    REGISTER_CONFIG_TYPE_FUNCTIONS(TYPE_CONFIG_BULLET, bullet_config_deserialize, NULL);
    REGISTER_CONFIG_TYPE_FUNCTIONS(TYPE_CONFIG_BULLET_SOURCE, bullet_source_config_deserialize, NULL);
    REGISTER_CONFIG_TYPE_FUNCTIONS(TYPE_CONFIG_TWEEN, tween_config_deserialize, NULL);
    REGISTER_CONFIG_TYPE_FUNCTIONS(TYPE_CONFIG_PARTICLE_EMITTER, particle_emitter_config_deserialize, NULL);
    REGISTER_CONFIG_TYPE_FUNCTIONS(TYPE_CONFIG_LEVEL_MANAGER, level_manager_config_deserialize, level_manager_config_cleanup);
    REGISTER_CONFIG_TYPE_FUNCTIONS(TYPE_CONFIG_LEVEL, level_config_deserialize, NULL);
}

void config_terminate() {
    config_system_terminate(&defaultConfigs);
}

bool config_load(const char* filename) {
    return config_system_load(&defaultConfigs, filename);
}

void config_reload_all() {
    config_system_reload_all(&defaultConfigs);
}

Config* config_get(const char* name) {
    return config_system_get(&defaultConfigs, name);
}

bool config_system_load(ConfigSystem* self, const char* filename) {
    ASSERT(self->rootDir, "Call config_init() before attempting to load configs!");

    char fullPath[256];
    IF_DEBUG(bool concatResult = )path_concat(self->rootDir, filename, fullPath, 256);
    ASSERT(concatResult, "Failed to concatenate paths, destination string not long enough.");

    void* element = hashtable_get(&self->configTable, filename);
    if (element) {
        // Already loaded so lets exit
        return false;
    }

    log_info_format("Config", "Loaded %s: %s", self->fileDescriptor, filename);
    Config* newConfig = CALLOC(1, Config);
    ini_load(&newConfig->data, fullPath);
    newConfig->path = CALLOC(strlen(fullPath) + 1, char);
    strcpy(newConfig->path, fullPath);
    newConfig->lastMTime = config_get_mtime(newConfig->path);

    hashtable_init(&newConfig->typeConfigs, 32, type_config_free_void);
    hashtable_insert(&self->configTable, filename, (void*)newConfig);

    return true;
}

void config_system_reload_all(ConfigSystem* self) {
    Config* configs[128];
    u32 count = (u32)hashtable_get_all(&self->configTable, (void**)configs, 128);

    for (u32 i = 0; i < count; ++i)
    {
        Config* config = (Config*)configs[i];

        if (config_type_update_mtime(config))
        {
            config_type_reload(config);
        }
    }
}

Config* config_system_get(ConfigSystem* self, const char* name) {
    Config* cfg = (Config*)hashtable_get(&self->configTable, name);
    ASSERT(cfg, "Config has not been loaded");
    return cfg;
}

time_t config_get_mtime(const char* path) {
    struct stat buffer;

    if (stat(path, &buffer) != 0) {
        ASSERT(false, "TODO: handle this gracefully.");
        return 0;
    }

    return buffer.st_mtime;
}

bool config_extract_config_file(const char* path, char* dest, size_t n) {
    int len = (int)strlen(path);

    int barIndex = -1;
    for (int i = 0; i < len; ++i) {
        if (path[i] == '|') {
            barIndex = i;
            break;
        }
    }

    if (barIndex < 0) {
        return false;
    }

    if (barIndex + 4 >= n) {
        return false;
    }

    strncpy(dest, path, barIndex);
    strncpy(&dest[barIndex], ".ini\0", 5);
    return true;
}

bool config_extract_section(const char* path, char* dest, size_t n) {
    int len = (int)strlen(path);
    
    int barIndex = -1;
    int colonIndex = -1;
    for (int i = 0; i < len; ++i) {
        char c = path[i];
        if (c == '|') {
            ASSERT(barIndex < 0, "Only 1 \'|\' should exist in the config path.");
            barIndex = i;
        } else if (c == ':') {
            colonIndex = i;
            break;
        }
    }

    // If we can't find a | or a : we have no way of determining if what we're looking at is a section or a key.
    if (barIndex < 0 && colonIndex < 0) {
        return false;
    }
    
    // Brings the bar index to 0 if one was never found and moves it off the | if we did find one.
    int start = barIndex + 1;
    // If we didn't find a colon but have a bar we can assume we go to the end of the string from bar.
    int end = (colonIndex < 0) ? len : colonIndex;

    if ((end - start) >= n) {
        return false;
    }

    strncpy(dest, &path[start], (end - start));
    dest[end - start] = '\0';

    return true;
}

bool config_extract_key(const char* path, char* dest, size_t n) {
    int len = (int)strlen(path);
    int start = len;
    for (int i = len - 1; i >= 0; ++i) {
        char c = path[i];
        if (c != ':') {
            --start;
        } else {
            break;
        }
    }

    if (start >= len) {
        return false;
    }

    if ((len - start) >= n) {
        return false;
    }

    strncpy(dest, &path[start], (len - start));
    dest[len] = '\0';

    return true;
}

Config* config_get_from_key(const char* key) {
    char configName[128];
    if (config_extract_config_file(key, configName, 128)) {
        return config_get(configName);
    } else {
        return NULL;
    }
}

CONFIG_IMPLEMENT_TYPE_ALIAS(i32, int);
CONFIG_IMPLEMENT_TYPE_ALIAS(u32, int);
CONFIG_IMPLEMENT_TYPE_ALIAS(f32, float);

CONFIG_GET_AT_PROTO_NAMED(char*, string) {
    return ini_get_string_at(&self->data, section, key, index);
}

CONFIG_TRY_GET_AT_PROTO_NAMED(char*, string) {
    return ini_try_get_string_at(&self->data, section, key, index, defaultValue);
}

CONFIG_GET_AT_PROTO(int) {
    return ini_get_int_at(&self->data, section, key, index);
}

CONFIG_TRY_GET_AT_PROTO(int) {
    return ini_try_get_int_at(&self->data, section, key, index, defaultValue);
}

CONFIG_GET_AT_PROTO(float) {
    return ini_get_float_at(&self->data, section, key, index);
}

CONFIG_TRY_GET_AT_PROTO(float) {
    return ini_try_get_float_at(&self->data, section, key, index, defaultValue);
}

CONFIG_GET_AT_PROTO(bool) {
    return ini_get_bool_at(&self->data, section, key, index);
}

CONFIG_TRY_GET_AT_PROTO(bool) {
    return ini_try_get_bool_at(&self->data, section, key, index, defaultValue);
}

CONFIG_GET_AT_PROTO(Vec2) {
    f32 x = CONFIG_GET_AT(float)(self, section, key, index * 2 + 0);
    f32 y = CONFIG_GET_AT(float)(self, section, key, index * 2 + 1);
    return vec2_init(x, y);
}

CONFIG_TRY_GET_AT_PROTO(Vec2) {
    f32 x = CONFIG_TRY_GET_AT(float)(self, section, key, index * 2 + 0, defaultValue.x);
    f32 y = CONFIG_TRY_GET_AT(float)(self, section, key, index * 2 + 1, defaultValue.y);
    return vec2_init(x, y);
}

CONFIG_GET_AT_PROTO(Range) {
    f32 min = CONFIG_GET_AT(float)(self, section, key, index * 2 + 0);
    f32 max = CONFIG_GET_AT(float)(self, section, key, index * 2 + 1);
    Range r;
    range_init(&r, min, max);
    return r;
}

CONFIG_TRY_GET_AT_PROTO(Range) {
    f32 min = CONFIG_TRY_GET_AT(float)(self, section, key, index * 2 + 0, defaultValue.min);
    f32 max = CONFIG_TRY_GET_AT(float)(self, section, key, index * 2 + 1, defaultValue.max);
    Range r;
    range_init(&r, min, max);
    return r;
}

CONFIG_GET_AT_PROTO(RandomRange) {
    if (config_is_array(self, section, key)) {
        f32 min = CONFIG_GET_AT(float)(self, section, key, index * 2 + 0);
        f32 max = CONFIG_GET_AT(float)(self, section, key, index * 2 + 1);
        RandomRange r;
        random_range_init(&r, min, max);
        return r;
    } else {
        f32 v = CONFIG_GET_AT(float)(self, section, key, index);
        RandomRange r;
        random_range_init(&r, v, v);
        return r;
    }
}

CONFIG_TRY_GET_AT_PROTO(RandomRange) {
    if (config_is_array(self, section, key)) {
        f32 min = CONFIG_TRY_GET_AT(float)(self, section, key, index * 2 + 0, defaultValue.min);
        f32 max = CONFIG_TRY_GET_AT(float)(self, section, key, index * 2 + 1, defaultValue.max);
        RandomRange r;
        random_range_init(&r, min, max);
        return r;
    } else {
        f32 v = CONFIG_TRY_GET_AT(float)(self, section, key, index, defaultValue.min);
        RandomRange r;
        random_range_init(&r, v, v);
        return r;
    }
}

CONFIG_GET_AT_PROTO(Color) {
    u8 r = (u8)CONFIG_GET_AT(int)(self, section, key, index * 2 + 0);
    u8 g = (u8)CONFIG_GET_AT(int)(self, section, key, index * 2 + 1);
    u8 b = (u8)CONFIG_GET_AT(int)(self, section, key, index * 2 + 2);
    u8 a = (u8)CONFIG_TRY_GET_AT(int)(self, section, key, index * 2 + 3, 255);
    Color result;
    color_init_rgba(&result, r, g, b, a);
    return result;
}

CONFIG_TRY_GET_AT_PROTO(Color) {
    u8 r = (u8)CONFIG_TRY_GET_AT(int)(self, section, key, index * 2 + 0, defaultValue.r);
    u8 g = (u8)CONFIG_TRY_GET_AT(int)(self, section, key, index * 2 + 1, defaultValue.g);
    u8 b = (u8)CONFIG_TRY_GET_AT(int)(self, section, key, index * 2 + 2, defaultValue.b);
    u8 a = (u8)CONFIG_TRY_GET_AT(int)(self, section, key, index * 2 + 3, defaultValue.a);
    Color result;
    color_init_rgba(&result, r, g, b, a);
    return result;
}

CONFIG_GET_AT_PROTO(dynf32) {
    dynf32 result;
    dynf32_zero(&result);

    char* str = config_get_string_at(self, section, key, index);

    char* endptr;
    result.value = strtof(str, &endptr);

    if (*endptr == '\0') {
        result.type = DYN_F32_VALUE;
        return result;
    }

    result.tweenConfig = CONFIG_GET_AT(TweenConfig)(self, section, key, index);
    result.tween = NULL;
    result.type = DYN_F32_TWEEN;
    return result;
}

CONFIG_TRY_GET_AT_PROTO(dynf32) {
    if (!config_try_get_string_at(self, section, key, index, NULL)) { return defaultValue; }

    return CONFIG_GET_AT(dynf32)(self, section, key, index);
}

CONFIG_GET_AT_PROTO(DynFloat2) {
    DynFloat2 result;
    result.x = CONFIG_GET_AT(dynf32)(self, section, key, index + 0);
    result.y = CONFIG_GET_AT(dynf32)(self, section, key, index + 1);
    return result;
}

CONFIG_TRY_GET_AT_PROTO(DynFloat2) {
    DynFloat2 result;
    result.x = CONFIG_TRY_GET_AT(dynf32)(self, section, key, index + 0, defaultValue.x);
    result.y = CONFIG_TRY_GET_AT(dynf32)(self, section, key, index + 1, defaultValue.y);
    return result;
}

CONFIG_GET_AT_PROTO(DynFloat3) {
    DynFloat3 result;
    result.x = CONFIG_GET_AT(dynf32)(self, section, key, index + 0);
    result.y = CONFIG_GET_AT(dynf32)(self, section, key, index + 1);
    result.z = CONFIG_GET_AT(dynf32)(self, section, key, index + 2);
    return result;
}

CONFIG_TRY_GET_AT_PROTO(DynFloat3) {
    DynFloat3 result;
    result.x = CONFIG_TRY_GET_AT(dynf32)(self, section, key, index + 0, defaultValue.x);
    result.y = CONFIG_TRY_GET_AT(dynf32)(self, section, key, index + 1, defaultValue.y);
    result.z = CONFIG_TRY_GET_AT(dynf32)(self, section, key, index + 2, defaultValue.z);
    return result;
}

CONFIG_GET_AT_PROTO(DynFloat4) {
    DynFloat4 result;
    result.x = CONFIG_GET_AT(dynf32)(self, section, key, index + 0);
    result.y = CONFIG_GET_AT(dynf32)(self, section, key, index + 1);
    result.z = CONFIG_GET_AT(dynf32)(self, section, key, index + 2);
    result.w = CONFIG_GET_AT(dynf32)(self, section, key, index + 3);
    return result;
}

CONFIG_TRY_GET_AT_PROTO(DynFloat4) {
    DynFloat4 result;
    result.x = CONFIG_TRY_GET_AT(dynf32)(self, section, key, index + 0, defaultValue.x);
    result.y = CONFIG_TRY_GET_AT(dynf32)(self, section, key, index + 1, defaultValue.y);
    result.z = CONFIG_TRY_GET_AT(dynf32)(self, section, key, index + 2, defaultValue.z);
    result.w = CONFIG_TRY_GET_AT(dynf32)(self, section, key, index + 3, defaultValue.w);
    return result;
}

CONFIG_GET_AT_PROTO(DynamicVec2) {
    char* table = CONFIG_GET_AT(string)(self, section, key, index);
    
    if (config_is_array(self, section, key)) {
        dynf32 x = CONFIG_GET_AT(dynf32)(self, section, key, 0);
        dynf32 y = CONFIG_GET_AT(dynf32)(self, section, key, 1);
        return dynamic_vec2_dynfloat2(x, y);
    }

    DynamicVec2 result;
    result.time = CONFIG_GET_AT(dynf32)(self, table, "time", index);
    result.min = CONFIG_GET_AT(DynFloat2)(self, table, "min", index);
    result.max = CONFIG_GET_AT(DynFloat2)(self, table, "max", index);
    return result;
}

CONFIG_TRY_GET_AT_PROTO(DynamicVec2) {
    if (!CONFIG_TRY_GET_AT(string)(self, section, key, index, NULL)) { return defaultValue; }
    return CONFIG_GET_AT(DynamicVec2)(self, section, key, index);
}

CONFIG_GET_AT_PROTO(DynamicColor) {
    char* table = CONFIG_GET_AT(string)(self, section, key, index);

    if (config_is_array(self, section, key)) {
        u8 rb = (u8)CONFIG_GET_AT(int)(self, section, key, 0);
        u8 gb = (u8)CONFIG_GET_AT(int)(self, section, key, 1);
        u8 bb = (u8)CONFIG_GET_AT(int)(self, section, key, 2);
        u8 ab = (u8)CONFIG_GET_AT(int)(self, section, key, 3);
        dynf32 r = dynf32_value((f32)rb);
        dynf32 g = dynf32_value((f32)gb);
        dynf32 b = dynf32_value((f32)bb);
        dynf32 a = dynf32_value((f32)ab);
        return dynamic_color_dynfloat4(r, g, b, a);
    }

    DynamicColor result;
    result.time = CONFIG_GET_AT(dynf32)(self, table, "time", index);
    result.min = CONFIG_GET_AT(DynFloat4)(self, table, "min", index);
    result.max = CONFIG_GET_AT(DynFloat4)(self, table, "max", index);
    return result;
}

CONFIG_TRY_GET_AT_PROTO(DynamicColor) {
    if (!CONFIG_TRY_GET_AT(string)(self, section, key, index, NULL)) { return defaultValue; }
    return CONFIG_GET_AT(DynamicColor)(self, section, key, index);
}

CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(ColliderConfig, TYPE_CONFIG_COLLIDER);
CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(BulletConfig, TYPE_CONFIG_BULLET);
CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(BulletSourceConfig, TYPE_CONFIG_BULLET_SOURCE);
CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(TweenConfig, TYPE_CONFIG_TWEEN);
CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(ParticleEmitterConfig, TYPE_CONFIG_PARTICLE_EMITTER);
CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(LevelManagerConfig, TYPE_CONFIG_LEVEL_MANAGER);
CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(LevelConfig, TYPE_CONFIG_LEVEL);