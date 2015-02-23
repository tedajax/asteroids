#include "luacomponent.h"
#include "snprintf.h"

bool lua_file_update_mtime(LuaFile* self) {
    profiler_tick("update lua mtime");
    struct stat buffer;
    if (stat(self->path, &buffer) != 0) {
        //ASSERT(false, "Failed to get stat of file.");
        return false;
    }
    profiler_tock("update lua mtime");
    time_t mtime = buffer.st_mtime;

    if (mtime > self->lastMTime) {
        self->lastMTime = mtime;
        return true;
    }

    return false;
}

LuaComponent* lua_component_new(Entity entity, const char* filename) {
    LuaComponent* self = CALLOC(1, LuaComponent);

    self->file.path = CALLOC(strlen(filename) + 1, char);
    strcpy(self->file.path, filename);
    self->file.lastMTime = 0;
    lua_file_update_mtime(&self->file);

    component_init(&self->super, COMPONENT_LUA, sizeof(LuaComponent), entity);

    lua_component_load(self);

    self->state = LUA_STATE_NEW;

    return self;
}

void lua_component_load(LuaComponent* self) {
    // TODO: error handling
    self->L = lua_open();
    luaL_openlibs(self->L);
    if (luaL_dofile(self->L, self->file.path)) {
        printf("%s\n", lua_tostring(self->L, -1));
    }

    {
        LuaBind* bind = &self->callbackBinds[LUA_CALLBACK_START];
        lua_bind_init(bind, "start", 1, LUA_ARG_INTEGER);
    }

    {
        LuaBind* bind = &self->callbackBinds[LUA_CALLBACK_UPDATE];
        lua_bind_init(bind, "update", 1, LUA_ARG_FLOAT);
    }

    {
        LuaBind* bind = &self->callbackBinds[LUA_CALLBACK_RENDER];
        lua_bind_init(bind, "render", 0);
    }

    hashtable_init(&self->customBinds, 16, lua_bind_free_void);
}


COMPONENT_DESERIALIZE(COMPONENT_LUA) {
    char* filename = CONFIG_GET(string)(config, table, "file");
    //TODO: This scripts path should be pulled from elsewhere
    char path[128];
    snprintf(path, 128, "%s/%s", "assets/scripts", filename);
    return (Component*)lua_component_new(0, path);
}

COMPONENT_FREE(COMPONENT_LUA) {
    LuaComponent* self = (LuaComponent*)component;
    lua_close(self->L);
    free(self->file.path);
    for (LuaComponentCallbacks cb = 0; cb < LUA_CALLBACK_LAST; ++cb) {
        lua_bind_free(&self->callbackBinds[cb]);
    }
    hashtable_free_contents(&self->customBinds);
}

COMPONENT_COPY(COMPONENT_LUA) {
    LuaComponent* luaSrc = (LuaComponent*)source;
    LuaComponent* luaDest = (LuaComponent*)dest;

    size_t len = strlen(luaSrc->file.path) + 1;
    luaDest->file.path = CALLOC(len, char);
    strcpy(luaDest->file.path, luaSrc->file.path);

    lua_component_load(luaDest);
}

void lua_component_check_and_reload(LuaComponent* self) {
    if (lua_file_update_mtime(&self->file)) {
        profiler_tick("reload lua file");
        lua_close(self->L);
        self->L = lua_open();
        luaL_openlibs(self->L);
        luaL_dofile(self->L, self->file.path);
        self->state = LUA_STATE_NEW;
        profiler_tock("reload lua file");
    }
}

void lua_component_add_bind(LuaComponent* self, const char* funcName, int argc, ...) {
    LuaBind* bind = CALLOC(1, LuaBind);
    
    va_list argv;
    va_start(argv, argc);
    
    lua_bind_initv(bind, funcName, argc, argv);
    
    va_end(argv);

    hashtable_insert(&self->customBinds, funcName, bind);
}

void lua_component_call(LuaComponent* self, const char* funcName, ...) {
    va_list argv;
    va_start(argv, funcName);
    lua_component_callv(self, funcName, argv);
    va_end(argv);
}

void lua_component_callv(LuaComponent* self, const char* funcName, va_list argv) {
    LuaBind* bind = (LuaBind*)hashtable_get(&self->customBinds, funcName);

    if (!bind) {
        log_error_format("Lua", "No bind found for \'%s\' in file \'%s\'", funcName, self->file.path);
        return;
    }

    lua_bind_callv(bind, self->L, argv);
}