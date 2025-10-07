#include "Signal.h"

void Signal::ConnectLua(lua_State* state, int funcIndex) {
    if (!state) return;
    if (!L) L = state;
    lua_pushvalue(state, funcIndex);
    int ref = lua_ref(state, LUA_REGISTRYINDEX);
    LuaConnections.push_back(ref);
}

void Signal::ConnectCpp(const std::function<void()>& cb) {
    CppConnections.push_back(cb);
}

void Signal::Fire() {
    for (auto& cb : CppConnections) cb();

    if (!L) return;
    for (int ref : LuaConnections) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
        lua_pushnil(L); // no arguments
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            printf("Signal Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
}

void Signal::Fire(const std::string& s) {
    for (auto& cb : CppConnections) cb();

    if (!L) return;
    for (int ref : LuaConnections) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
        lua_pushstring(L, s.c_str());
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            printf("Signal Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
}

void Signal::Fire(double n) {
    for (auto& cb : CppConnections) cb();

    if (!L) return;
    for (int ref : LuaConnections) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
        lua_pushnumber(L, n);
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            printf("Signal Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
}

void Signal::Fire(bool b) {
    for (auto& cb : CppConnections) cb();

    if (!L) return;
    for (int ref : LuaConnections) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
        lua_pushboolean(L, b);
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            printf("Signal Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
}

void Signal::Fire(Instance* inst) {
    for (auto& cb : CppConnections) cb();

    if (!L) return;
    for (int ref : LuaConnections) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
        lua_pushlightuserdata(L, inst);
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            printf("Signal Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
}

void Signal::DisconnectAll() {
  for (int ref : LuaConnections)
    if (L)
      lua_unref(L, ref);

  LuaConnections.clear();
  CppConnections.clear();
}
