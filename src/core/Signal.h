#pragma once

#include <functional>
#include <variant>
#include <optional>
#include <vector>
#include <string>

#include "../../luau/VM/include/lua.h"
#include "../../luau/VM/include/lualib.h"
#include "../../luau/Compiler/include/luacode.h"

struct Instance;

using SignalArg = std::variant<std::monostate, std::string, bool, double, Instance>;

struct Connection {
    bool Connected = true;
    std::function<void(SignalArg)> Callback;

    void Disconnect() { Connected = false; }
};

struct Signal {
    lua_State* L = nullptr;
    std::vector<int> LuaConnections; // LUA registry refs
    std::vector<std::function<void()>> CppConnections; // optional

    // Lua
    void ConnectLua(lua_State* L, int funcIndex);

    // C++
    void ConnectCpp(const std::function<void()>& cb);

    // Fire
    void Fire(); // no args
    void Fire(const std::string& s);
    void Fire(double n);
    void Fire(bool b);
    void Fire(Instance* inst);

    void DisconnectAll();
};
