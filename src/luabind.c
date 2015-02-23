#include "luabind.h"
#include "memory.h"

void lua_bind_init(LuaBind* self, const char* function, int argc, ...) {
    self->functionName = (char*)function;
    self->argt = NULL;
    self->argc = argc;

    if (self->argc > 0) {
        self->argt = CALLOC(argc, LuaBindArgTypes);
    }

    va_list argv;
    va_start(argv, argc);

    for (int i = 0; i < argc; ++i) {
        self->argt[i] = (LuaBindArgTypes)va_arg(argv, int);
    }

    va_end(argv);
}

void lua_bind_call(LuaBind* self, lua_State* L, ...) {
    va_list argv;
    va_start(argv, L);

    lua_bind_callv(self, L, argv);

    va_end(argv);
}

void lua_bind_callv(LuaBind* self, lua_State* L, va_list argv) {
    lua_getglobal(L, self->functionName);

    if (lua_isnil(L, -1)) {
        lua_pop(L, -1);
        return;
    }

    for (int i = 0; i < self->argc; ++i) {
        switch (self->argt[i]) {
            case LUA_ARG_BOOLEAN:
                lua_pushboolean(L, va_arg(argv, int));
                break;

            case LUA_ARG_INTEGER:
                lua_pushinteger(L, va_arg(argv, int));
                break;

            case LUA_ARG_FLOAT:
                lua_pushnumber(L, va_arg(argv, double));
                break;

            case LUA_ARG_STRING:
                lua_pushstring(L, va_arg(argv, char*));
                break;
        }
    }

    if (lua_pcall(L, self->argc, 0, 0) != 0) {
        fprintf(stderr, "Lua error -- %s:%d -- %s: %s\n", __FILE__, __LINE__, self->functionName, lua_tostring(L, -1));
        LUA_ERROR(L);
    }
}

void lua_bind_free(LuaBind* self) {
    if (self->argt) {
        free(self->argt);
    }
}