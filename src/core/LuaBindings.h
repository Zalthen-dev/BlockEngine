#pragma once

#include <vector>
#include <string>
#include <sstream>

#include "../../dependencies/luau/VM/include/lua.h"
#include "../../dependencies/luau/VM/include/lualib.h"
#include "../../dependencies/luau/Compiler/include/luacode.h"

#include "../datatypes/Vector3.h"
#include "../datatypes/Color3.h"
#include "../datatypes/Instance.h"
#include "../datatypes/Task.h"

#include "../instances/BasePart.h"
#include "../instances/Part.h"

#include "Signal.h"
#include "Logger.h"

#include "raylib.h"
#include "raymath.h"

extern std::vector<std::string> luaOutput;
extern Logger logger;

void RegisterScriptBindings(lua_State* L);
