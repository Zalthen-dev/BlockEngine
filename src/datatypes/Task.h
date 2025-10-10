#pragma once
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <functional>

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

    LuaTask(lua_State* L)
    : thread(lua_newthread(L)) {}
};

struct SleepingTask {
    lua_State* thread;
    double wakeTime; // in seconds
};

extern std::vector<LuaTask> g_tasks;

int Task_RunScript(lua_State* L, std::string& scriptText);

void TaskScheduler_Step();
void Task_Bind(lua_State* L);
