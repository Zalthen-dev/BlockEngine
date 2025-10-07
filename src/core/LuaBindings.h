#pragma once

#include <vector>

#include "../../luau/VM/include/lua.h"
#include "../../luau/VM/include/lualib.h"
#include "../../luau/Compiler/include/luacode.h"

#include "../datatypes/Vector3.h"
#include "../datatypes/Color3.h"
#include "../datatypes/Instance.h"
#include "../datatypes/Task.h"

#include "../instances/BasePart.h"
#include "../instances/Part.h"

#include "Signal.h"

#include "raylib.h"
#include "raymath.h"

namespace LuaBindings {
    extern std::vector<BasePart*>* g_instances;
    extern Camera3D* gg_camera;

    int Lua_SpawnPart(lua_State* L);
    int Lua_SetCameraPos(lua_State* L);

    void RegisterScriptBindings(lua_State* L, std::vector<BasePart*>& parts, Camera3D& g_camera);
}
