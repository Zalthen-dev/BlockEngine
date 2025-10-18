#pragma once
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <memory>

#include "raylib.h"
#include "raymath.h"

#include "../../dependencies/luau/VM/include/lua.h"
#include "../../dependencies/luau/VM/include/lualib.h"
#include "../../dependencies/luau/Compiler/include/luacode.h"

struct LuaTask {
    lua_State* thread;
    double WakeTime = 0.0;
    double SleepStartTime = 0.0;
    bool Finished = false;
    bool ShouldStop = false;

    LuaTask(lua_State* L) {
        thread = lua_newthread(L);
        lua_pop(L, 1);
        luaL_sandboxthread(thread);
    }
};

extern std::vector<std::unique_ptr<LuaTask>> g_tasks;

std::unique_ptr<LuaTask> Task_Run(lua_State* L, std::string& scriptText);
int Task_TryRun(lua_State* L, std::string& scriptText);

void TaskScheduler_Step(void);
void Task_Bind(lua_State* L);
