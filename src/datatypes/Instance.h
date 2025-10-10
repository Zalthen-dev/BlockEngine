#pragma once
#include <string.h>
#include <vector>
#include <raylib.h>

#include "../../dependencies/luau/VM/include/lua.h"
#include "../../dependencies/luau/VM/include/lualib.h"
#include "../../dependencies/luau/Compiler/include/luacode.h"

#include "../instances/Part.h"

extern std::vector<BasePart*> g_instances;
void Instance_Bind(lua_State* L);
