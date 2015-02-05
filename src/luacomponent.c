#include "luacomponent.h"

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

    self->file.path = (char*)filename;
    self->file.lastMTime = 0;
    lua_file_update_mtime(&self->file);

    component_init(&self->super, COMPONENT_LUA, sizeof(LuaComponent), entity);

    // TODO: error handling
    self->L = lua_open();
    luaL_openlibs(self->L);
    luaL_dofile(self->L, self->file.path);

    {
        LuaBind* bind = &self->callbackBinds[LUA_CALLBACK_START];
        lua_bind_init(bind, "start", 0);
    }

    {
        LuaBind* bind = &self->callbackBinds[LUA_CALLBACK_UPDATE];
        lua_bind_init(bind, "update", 1, LUA_ARG_FLOAT);
    }

    {
        LuaBind* bind = &self->callbackBinds[LUA_CALLBACK_RENDER];
        lua_bind_init(bind, "render", 0);
    }

    self->state = LUA_STATE_NEW;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_LUA) {
    char* filename = CONFIG_GET(string)(config, table, "filename");
    return (Component*)lua_component_new(0, filename);
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

void lua_component_free(LuaComponent* self) {
    lua_close(self->L);
    free(self);
}