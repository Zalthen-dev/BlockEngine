#include "Task.h"

extern lua_State* L_main;
std::vector<std::unique_ptr<LuaTask>> g_tasks;

static const char* luaGlobals[] = {
    "game", "workspace", "script", "shared", "plugin", nullptr
};

std::unique_ptr<LuaTask> Task_Run(lua_State* L, std::string& scriptText) {
    size_t bcSize;
    lua_CompileOptions opts{};
    opts.optimizationLevel = 1;
    opts.debugLevel = 1;
    opts.mutableGlobals = luaGlobals;

    const char* bytecode = luau_compile(scriptText.c_str(), scriptText.size(), &opts, &bcSize);
    if (!bytecode || bcSize == 0) {
        if (bytecode) free((void*)bytecode);
        return 0;
    }

    auto task = std::make_unique<LuaTask>(L);
    lua_State* thread = task->thread;

    int loadStatus = luau_load(thread, "ScriptChunk", bytecode, bcSize, 0);
    free((void*)bytecode);

    if (loadStatus != LUA_OK) {
        const char* err = lua_tostring(thread, -1);
        printf("Error loading script: %s\n", err);
        lua_pop(thread, 1);
        return 0;
    }

    task->WakeTime = GetTime();
    g_tasks.push_back(std::move(task));

    return task;
}

int Task_TryRun(lua_State* L, std::string& scriptText) {
    std::unique_ptr<LuaTask> task = Task_Run(L, scriptText);
    if (!task) {
        return 0;
    }

    return 1;
}

static int Task_Spawn(lua_State* L) {
    luaL_checktype(L, 1, LUA_TFUNCTION);

    auto task = std::make_unique<LuaTask>(L);
    lua_State* thread = task->thread;

    lua_pushvalue(L, 1);
    lua_xmove(L, thread, 1);

    g_tasks.push_back(std::move(task));

    return 0;
}

static int Task_Wait(lua_State* L) {
    double delay = luaL_optnumber(L, 1, 0.0);
    double now = GetTime();

    for (auto& task : g_tasks) {
        if (task->thread == L) {
            task->SleepStartTime = now;
            task->WakeTime = now + delay;

            if (task->ShouldStop) {
                task->Finished = true;
                lua_error(L);
                return 0;
            }

            return lua_yield(L, 0);
        }
    }

    luaL_error(L, "attempted to use task.wait outside of a running task");
    return 0;
}

void TaskScheduler_Step() {
    double now = GetTime();

    //lua_gc(L_main, LUA_GCSTEP, 200);

    for (auto& task : g_tasks) {
        if (task->Finished) continue;

        if (now >= task->WakeTime) {
            double elapsed = now - task->SleepStartTime;
            lua_pushnumber(task->thread, elapsed);

            int status = lua_resume(task->thread, nullptr, 1);

            if (status == LUA_YIELD) {
                if (task->ShouldStop) task->Finished = true;
            } else if (status == LUA_OK) {
                task->Finished = true;
            } else {
                printf("Lua error: %s\n", lua_tostring(task->thread, -1));
                lua_pop(task->thread, 1);
                task->Finished = true;
            }
        }
    }

    g_tasks.erase(
        std::remove_if(
            g_tasks.begin(),
            g_tasks.end(),
            [](const std::unique_ptr<LuaTask>& t){ return t->Finished; }
        ),
        g_tasks.end()
    );
}

void Task_Bind(lua_State* L) {
    lua_newtable(L);

    lua_pushcfunction(L, Task_Spawn, "spawn"); lua_setfield(L, -2, "spawn");
    lua_pushcfunction(L, Task_Wait, "wait"); lua_setfield(L, -2, "wait");

    lua_setglobal(L, "task");

    lua_newtable(L);
    lua_setfield(L, LUA_REGISTRYINDEX, "_TASK_THREADS");
}

