#include "LuaBindings.h"

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
    std::vector<BasePart*>* g_instances = nullptr;
    Camera3D* gg_camera = nullptr;

    int Lua_SetCameraPos(lua_State* L) {
        float x = (float)lua_tonumber(L, 1);
        float y = (float)lua_tonumber(L, 2);
        float z = (float)lua_tonumber(L, 3);

        gg_camera->position = {x, y, z};

        return 0;
    }

    void RegisterScriptBindings(lua_State* L, std::vector<BasePart*>& parts, Camera3D& g_camera) {
        g_instances = &parts;
        gg_camera = &g_camera;

        lua_newtable(L);

        lua_pushcfunction(L, Lua_SetCameraPos, "SetCameraPos"); lua_setfield(L, -2, "SetCameraPos");

        lua_setglobal(L, "Engine");

        Vector3Game_Bind(L);
        Color3_Bind(L);
        Instance_Bind(L);
        Task_Bind(L);

        BasePart_Bind(L);
        Part_Bind(L);

        Lua_RegisterSignal(L);
    }
}
