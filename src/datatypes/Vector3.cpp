#include "Vector3.h"

const Vector3Game Vector3_zero = Vector3Game{0, 0, 0};
const Vector3Game Vector3_one = Vector3Game{1, 1, 1};

const Vector3Game Vector3_xAxis = Vector3Game{1, 0, 0};
const Vector3Game Vector3_yAxis = Vector3Game{0, 1, 0};
const Vector3Game Vector3_zAxis = Vector3Game{0, 0, 1};

static int Vector3_new(lua_State* L) {
    float x = (float)luaL_optnumber(L, 1, 0);
    float y = (float)luaL_optnumber(L, 2, 0);
    float z = (float)luaL_optnumber(L, 3, 0);

    Vector3Game* v = (Vector3Game*)lua_newuserdata(L, sizeof(Vector3Game));
    *v = Vector3Game{x, y, z};

    luaL_getmetatable(L, "Vector3Meta");
    lua_setmetatable(L, -2);

    return 1;
}

static int Vector3_tostring(lua_State* L) {
    if (lua_isnumber(L, 1)) return 1;

    Vector3Game* v3 = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    lua_pushfstring(L, "%f, %f, %f", v3->x, v3->y, v3->z);

    return 1;
}

static int Vector3_add(lua_State* L) {
    Vector3Game* a = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    Vector3Game* b = (Vector3Game*)luaL_checkudata(L, 2, "Vector3Meta");

    Vector3Game* result = (Vector3Game*)lua_newuserdata(L, sizeof(Vector3Game));
    *result = Vector3Game{a->x + b->x, a->y + b->y, a->z + b->z};

    luaL_getmetatable(L, "Vector3Meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int Vector3_sub(lua_State* L) {
    Vector3Game* a = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    Vector3Game* b = (Vector3Game*)luaL_checkudata(L, 2, "Vector3Meta");

    Vector3Game* result = (Vector3Game*)lua_newuserdata(L, sizeof(Vector3Game));
    *result = Vector3Game{a->x - b->x, a->y - b->y, a->z - b->z};

    luaL_getmetatable(L, "Vector3Meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int Vector3_mul(lua_State* L) {
    Vector3Game* v;
    float s;

    if (lua_isnumber(L, 1)) {
        s = (float)luaL_checknumber(L, 1);
        v = (Vector3Game*)luaL_checkudata(L, 2, "Vector3Meta");
    } else {
        v = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
        s = (float)luaL_checknumber(L, 2);
    }

    Vector3Game* result = (Vector3Game*)lua_newuserdata(L, sizeof(Vector3Game));
    *result = Vector3Game{v->x * s, v->y * s, v->z * s};

    luaL_getmetatable(L, "Vector3Meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int Vector3_div(lua_State* L) {
    Vector3Game* v = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    float s = (float)luaL_checknumber(L, 2);

    if (s == 0.0f) {
        luaL_error(L, "division by zero");
        return 0;
    }


    Vector3Game* result = (Vector3Game*)lua_newuserdata(L, sizeof(Vector3Game));
    *result = Vector3Game{v->x / s, v->y / s, v->z / s};

    luaL_getmetatable(L, "Vector3Meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int Vector3_unm(lua_State* L) {
    Vector3Game* v = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");

    Vector3Game* result = (Vector3Game*)lua_newuserdata(L, sizeof(Vector3Game));
    *result = Vector3Game{-v->x, -v->y, -v->z};

    luaL_getmetatable(L, "Vector3Meta");
    lua_setmetatable(L, -2);
    return 1;
}


static int Vector3_index(lua_State* L) {
    Vector3Game* v3 = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    const char* key = luaL_checkstring(L, 2);

    if (strcmp(key, "X") == 0)
        lua_pushnumber(L, v3->x);

    else if (strcmp(key, "Y") == 0)
        lua_pushnumber(L, v3->y);

    else if (strcmp(key, "Z") == 0)
        lua_pushnumber(L, v3->z);

    else if (strcmp(key, "Magnitude") == 0)
        lua_pushnumber(L, v3->magnitude());

    else if (strcmp(key, "Unit") == 0) {
        Vector3Game v = v3->unit();
        luaL_getmetatable(L, "Vector3Meta");
        lua_setmetatable(L, -2);
        lua_pushlightuserdata(L, &v);
    } else {
        luaL_getmetatable(L, "Vector3Meta");
        lua_getfield(L, -1, "__index");
        lua_getfield(L, -1, key);

        if (lua_isnil(L, -1)) {
            luaL_error(L, "Attempt to access invalid property/method '%s' of Vector3", key);
            return 0;
        }
    }

    return 1;
}

static int Vector3_abs(lua_State* L) {
    Vector3Game* v3 = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    Vector3Game v = v3->abs();
    lua_pushlightuserdata(L, &v);
    return 1;
}

static int Vector3_ceil(lua_State* L) {
    Vector3Game* v3 = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    Vector3Game v = v3->ceil();
    lua_pushlightuserdata(L, &v);
    return 1;
}

static int Vector3_floor(lua_State* L) {
    Vector3Game* v3 = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    Vector3Game v = v3->floor();
    lua_pushlightuserdata(L, &v);
    return 1;
}

static int Vector3_dot(lua_State* L) {
    Vector3Game* v3a = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    Vector3Game* v3b = (Vector3Game*)luaL_checkudata(L, 2, "Vector3Meta");

    lua_pushnumber(L, v3a->dot(*v3b));

    return 1;
}

static int Vector3_cross(lua_State* L) {
    Vector3Game* v3a = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    Vector3Game* v3b = (Vector3Game*)luaL_checkudata(L, 2, "Vector3Meta");

    Vector3Game v = v3a->cross(*v3b);
    lua_pushlightuserdata(L, &v);

    return 1;
}

static int Vector3_lerp(lua_State* L) {
    Vector3Game* v3a = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    Vector3Game* v3b = (Vector3Game*)luaL_checkudata(L, 2, "Vector3Meta");
    float alpha = (float)luaL_checknumber(L, 3);

    Vector3Game v = v3a->lerp(*v3b, alpha);
    lua_pushlightuserdata(L, &v);

    return 1;
}

static int Vector3_fuzzyeq(lua_State* L) {
    Vector3Game* v3a = (Vector3Game*)luaL_checkudata(L, 1, "Vector3Meta");
    Vector3Game* v3b = (Vector3Game*)luaL_checkudata(L, 2, "Vector3Meta");

    lua_pushboolean(L, v3a->fuzzyequal(*v3b));
    return 1;
}

void Vector3Game_Bind(lua_State* L) {
    lua_newtable(L);

    auto pushVector3Const = [L](const char* name, const Vector3Game& v) {
        Vector3Game* ptr = (Vector3Game*)lua_newuserdata(L, sizeof(Vector3Game));
        *ptr = v;

        luaL_getmetatable(L, "Vector3Meta");
        lua_setmetatable(L, -2);
        lua_setfield(L, -2, name);
    };

    luaL_newmetatable(L, "Vector3Meta"); // Stack: [Vector3, mt]
    lua_pushcfunction(L, Vector3_index, "__index"); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, Vector3_tostring, "__tostring"); lua_setfield(L, -2, "__tostring");
    lua_pushcfunction(L, Vector3_add, "__add"); lua_setfield(L, -2, "__add");
    lua_pushcfunction(L, Vector3_sub, "__sub"); lua_setfield(L, -2, "__sub");
    lua_pushcfunction(L, Vector3_mul, "__mul"); lua_setfield(L, -2, "__mul");
    lua_pushcfunction(L, Vector3_div, "__div"); lua_setfield(L, -2, "__div");
    lua_pushcfunction(L, Vector3_unm, "__unm"); lua_setfield(L, -2, "__unm");

    lua_pushcfunction(L, Vector3_abs, "Abs"); lua_setfield(L, -2, "Abs");
    lua_pushcfunction(L, Vector3_ceil, "Ceil"); lua_setfield(L, -2, "Ceil");
    lua_pushcfunction(L, Vector3_floor, "Floor"); lua_setfield(L, -2, "Floor");

    lua_pushcfunction(L, Vector3_dot, "Dot"); lua_setfield(L, -2, "Dot");
    lua_pushcfunction(L, Vector3_cross, "Cross"); lua_setfield(L, -2, "Cross");
    lua_pushcfunction(L, Vector3_lerp, "Lerp"); lua_setfield(L, -2, "Lerp");
    lua_pushcfunction(L, Vector3_fuzzyeq, "FuzzyEq"); lua_setfield(L, -2, "FuzzyEq");
    lua_pop(L, 1);

    pushVector3Const("zero", Vector3_zero);
    pushVector3Const("one", Vector3_one);
    pushVector3Const("xAxis", Vector3_xAxis);
    pushVector3Const("yAxis", Vector3_yAxis);
    pushVector3Const("zAxis", Vector3_zAxis);

    lua_pushcfunction(L, Vector3_new, "new"); lua_setfield(L, -2, "new");
    lua_setglobal(L, "Vector3");
}
