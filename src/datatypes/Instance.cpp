#include "Instance.h"

std::vector<Object*> g_instances;

static int Instance_new(lua_State* L) {
    const char* className = luaL_checkstring(L, 1);

    if (strcmp(className, "Part") == 0) {
        Part* p = (Part*)lua_newuserdata(L, sizeof(Part));
        new (p) Part();

        luaL_getmetatable(L, "PartMeta");
        lua_setmetatable(L, -2);

        g_instances.push_back(p);
        return 1;
    }

    luaL_error(L, "Unknown instance type '%s'", className);
    return 0;
}

void Instance_Bind(lua_State* L) {
    lua_newtable(L);

    lua_pushcfunction(L, Instance_new, "new");
    lua_setfield(L, -2, "new");

    lua_setglobal(L, "Instance");
}
