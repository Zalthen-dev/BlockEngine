#include "Color3.h"

// --- Helper methods ---
Color3 Color3::fromHSV(float h, float s, float v) {
    h = fmodf(h, 1.0f) * 6.0f;
    int i = (int)floorf(h);
    float f = h - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i) {
        case 0: return Color3(v, t, p);
        case 1: return Color3(q, v, p);
        case 2: return Color3(p, v, t);
        case 3: return Color3(p, q, v);
        case 4: return Color3(t, p, v);
        default: return Color3(v, p, q);
    }
}

Color3 Color3::Lerp(const Color3& other, float alpha) const {
    alpha = std::clamp(alpha, 0.0f, 1.0f);
    return Color3(
        r + (other.r - r) * alpha,
                  g + (other.g - g) * alpha,
                  b + (other.b - b) * alpha
    );
}

static int Color3_new(lua_State* L) {
    float r = (float)luaL_checknumber(L, 1);
    float g = (float)luaL_checknumber(L, 2);
    float b = (float)luaL_checknumber(L, 3);

    Color3* c = (Color3*)lua_newuserdata(L, sizeof(Color3));
    *c = Color3(r, g, b);

    luaL_getmetatable(L, "Color3Meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int Color3_fromRGB(lua_State* L) {
    int r = luaL_checkinteger(L, 1);
    int g = luaL_checkinteger(L, 2);
    int b = luaL_checkinteger(L, 3);

    Color3* c = (Color3*)lua_newuserdata(L, sizeof(Color3));
    *c = Color3::fromRGB(r, g, b);

    luaL_getmetatable(L, "Color3Meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int Color3_fromHSV(lua_State* L) {
    float h = (float)luaL_checknumber(L, 1);
    float s = (float)luaL_checknumber(L, 2);
    float v = (float)luaL_checknumber(L, 3);

    Color3* c = (Color3*)lua_newuserdata(L, sizeof(Color3));
    *c = Color3::fromHSV(h, s, v);

    luaL_getmetatable(L, "Color3Meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int Color3_toRGB(lua_State* L) {
    Color3* c = (Color3*)luaL_checkudata(L, 1, "Color3Meta");

    lua_pushinteger(L, (int)(c->r * 255.0f));
    lua_pushinteger(L, (int)(c->g * 255.0f));
    lua_pushinteger(L, (int)(c->b * 255.0f));
    return 3;
}

static int Color3_lerp(lua_State* L) {
    Color3* a = (Color3*)luaL_checkudata(L, 1, "Color3Meta");
    Color3* b = (Color3*)luaL_checkudata(L, 2, "Color3Meta");
    float alpha = (float)luaL_checknumber(L, 3);

    Color3* result = (Color3*)lua_newuserdata(L, sizeof(Color3));
    *result = a->Lerp(*b, alpha);

    luaL_getmetatable(L, "Color3Meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int Color3_tostring(lua_State* L) {
    Color3* c = (Color3*)luaL_checkudata(L, 1, "Color3Meta");
    lua_pushfstring(L, "Color3(%.3f, %.3f, %.3f)", c->r, c->g, c->b);
    return 1;
}

static int Color3_add(lua_State* L) {
    Color3* a = (Color3*)luaL_checkudata(L, 1, "Color3Meta");
    Color3* b = (Color3*)luaL_checkudata(L, 2, "Color3Meta");

    Color3* result = (Color3*)lua_newuserdata(L, sizeof(Color3));
    *result = Color3(a->r + b->r, a->g + b->g, a->b + b->b);

    luaL_getmetatable(L, "Color3Meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int Color3_mul(lua_State* L) {
    Color3* a = (Color3*)luaL_checkudata(L, 1, "Color3Meta");
    float s = (float)luaL_checknumber(L, 2);

    Color3* result = (Color3*)lua_newuserdata(L, sizeof(Color3));
    *result = Color3(a->r * s, a->g * s, a->b * s);

    luaL_getmetatable(L, "Color3Meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int Color3_eq(lua_State* L) {
    Color3* a = (Color3*)luaL_checkudata(L, 1, "Color3Meta");
    Color3* b = (Color3*)luaL_checkudata(L, 2, "Color3Meta");

    lua_pushboolean(L, std::fabs(a->r - b->r) < 1e-6f &&
    std::fabs(a->g - b->g) < 1e-6f &&
    std::fabs(a->b - b->b) < 1e-6f);
    return 1;
}

void Color3_Bind(lua_State* L) {
    // Metatable
    luaL_newmetatable(L, "Color3Meta");

    lua_pushcfunction(L, Color3_tostring, "__tostring"); lua_setfield(L, -2, "__tostring");
    lua_pushcfunction(L, Color3_add, "__add"); lua_setfield(L, -2, "__add");
    lua_pushcfunction(L, Color3_mul, "__mul"); lua_setfield(L, -2, "__mul");
    lua_pushcfunction(L, Color3_eq, "__eq"); lua_setfield(L, -2, "__eq");

    lua_pop(L, 1);

    lua_newtable(L);

    lua_pushcfunction(L, Color3_new, "new"); lua_setfield(L, -2, "new");
    lua_pushcfunction(L, Color3_fromRGB, "fromRGB"); lua_setfield(L, -2, "fromRGB");
    lua_pushcfunction(L, Color3_fromHSV, "fromHSV"); lua_setfield(L, -2, "fromHSV");
    lua_pushcfunction(L, Color3_toRGB, "toRGB"); lua_setfield(L, -2, "toRGB");
    lua_pushcfunction(L, Color3_lerp, "lerp"); lua_setfield(L, -2, "lerp");

    lua_setglobal(L, "Color3");
}
