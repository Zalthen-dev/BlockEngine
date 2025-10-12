#include "LuaBindings.h"

std::vector<std::string> luaOutput;
Logger logger;

int lua_CapturedPrint(lua_State* L) {
    int n = lua_gettop(L);
    std::ostringstream oss;

    for (int i = 1; i <= n; ++i) {
        size_t len;
        const char* s = luaL_tolstring(L, i, &len);
        oss << s;
        if (i < n) oss << "\t";
        lua_pop(L, 1);
    }

    logger.Log(oss.str());
    return 0;
}

static int l_Signal_Connect(lua_State* L) {
    Signal* sig = *(Signal**)luaL_checkudata(L, 1, "Signal");
    luaL_checktype(L, 2, LUA_TFUNCTION);

    sig->ConnectLua(L, 2);
    return 0;
}

static int l_Signal_Fire(lua_State* L) {
    Signal* sig = *(Signal**)luaL_checkudata(L, 1, "Signal");
    const char* arg = luaL_optstring(L, 2, "");
    sig->Fire(arg);
    return 0;
}

static int l_Signal_DisconnectAll(lua_State* L) {
    Signal* sig = *(Signal**)luaL_checkudata(L, 1, "Signal");
    sig->DisconnectAll();
    return 0;
}

void Lua_RegisterSignal(lua_State* L) {
    luaL_newmetatable(L, "Signal");

    lua_pushcfunction(L, l_Signal_Connect, "Connect"); lua_setfield(L, -2, "Connect");

    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    lua_pop(L, 1);
}

namespace LuaBindings {
    void RegisterScriptBindings(lua_State* L) {
        lua_pushcfunction(L, lua_CapturedPrint, "print");
        lua_setglobal(L, "print");

        Vector3Game_Bind(L);
        Color3_Bind(L);
        Instance_Bind(L);
        Task_Bind(L);

        BasePart_Bind(L);
        Part_Bind(L);

        Lua_RegisterSignal(L);
    }
}
