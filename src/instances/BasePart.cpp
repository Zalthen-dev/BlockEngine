#include "BasePart.h"

static int BasePart_tostring(lua_State* L) {
    BasePart* part = (BasePart*)luaL_checkudata(L, 1, "BasePartMeta");
    char buf[256];
    snprintf(buf, sizeof(buf),
             "BasePart(%s, pos=(%.2f, %.2f, %.2f), size=(%.2f, %.2f, %.2f), color=(%.2f, %.2f, %.2f))",
             part->Name.c_str(),
             part->Position.x, part->Position.y, part->Position.z,
             part->Size.x, part->Size.y, part->Size.z,
             part->Color.r, part->Color.g, part->Color.b);

    lua_pushstring(L, buf);
    return 1;
}

static int BasePart_index(lua_State* L) {
    BasePart* part = (BasePart*)luaL_checkudata(L, 1, "BasePartMeta");
    const char* key = luaL_checkstring(L, 2);

    if (strcmp(key, "Name") == 0)
        lua_pushstring(L, part->Name.c_str());
    else if (strcmp(key, "ClassName") == 0)
        lua_pushstring(L, part->ClassName.c_str());
    else if (strcmp(key, "Anchored") == 0)
        lua_pushboolean(L, part->Anchored);
    else if (strcmp(key, "CanCollide") == 0)
        lua_pushboolean(L, part->CanCollide);
    else if (strcmp(key, "Transparency") == 0)
        lua_pushnumber(L, part->Transparency);
    else if (strcmp(key, "Position") == 0) {
        Vector3Game* v = (Vector3Game*)lua_newuserdata(L, sizeof(Vector3Game));
        *v = part->Position;
        luaL_getmetatable(L, "Vector3Meta");
        lua_setmetatable(L, -2);
    } else if (strcmp(key, "Rotation") == 0) {
        Vector3Game* v = (Vector3Game*)lua_newuserdata(L, sizeof(Vector3Game));
        *v = part->Rotation;
        luaL_getmetatable(L, "Vector3Meta");
        lua_setmetatable(L, -2);
    } else if (strcmp(key, "Size") == 0) {
        Vector3Game* v = (Vector3Game*)lua_newuserdata(L, sizeof(Vector3Game));
        *v = part->Size;
        luaL_getmetatable(L, "Vector3Meta");
        lua_setmetatable(L, -2);
    } else if (strcmp(key, "Color") == 0) {
        Color3* c = (Color3*)lua_newuserdata(L, sizeof(Color3));
        *c = part->Color;
        luaL_getmetatable(L, "Color3Meta");
        lua_setmetatable(L, -2);
    } else
        lua_pushnil(L);

    return 1;
}

static int BasePart_newindex(lua_State* L) {
    BasePart* part = (BasePart*)luaL_checkudata(L, 1, "BasePartMeta");
    const char* key = luaL_checkstring(L, 2);

    if (strcmp(key, "Name") == 0) {
        part->Name = luaL_checkstring(L, 3);
    } else if (strcmp(key, "Anchored") == 0) {
        part->Anchored = lua_toboolean(L, 3);
    } else if (strcmp(key, "Transparency") == 0) {
        part->Transparency = (float)luaL_checknumber(L, 3);
    } else if (strcmp(key, "Position") == 0) {
        Vector3Game* v = (Vector3Game*)luaL_checkudata(L, 3, "Vector3Meta");
        part->Position = *v;
    } else if (strcmp(key, "Rotation") == 0) {
        Vector3Game* v = (Vector3Game*)luaL_checkudata(L, 3, "Vector3Meta");
        part->Rotation = *v;
    } else if (strcmp(key, "Size") == 0) {
        Vector3Game* v = (Vector3Game*)luaL_checkudata(L, 3, "Vector3Meta");
        part->Size = *v;
    } else if (strcmp(key, "Color") == 0) {
        Color3* c = (Color3*)luaL_checkudata(L, 3, "Color3Meta");
        part->Color = *c;
    }

    return 0;
}

void BasePart_Bind(lua_State* L) {
    luaL_newmetatable(L, "BasePartMeta");

    lua_pushcfunction(L, BasePart_index, "__index"); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, BasePart_newindex, "__newindex"); lua_setfield(L, -2, "__newindex");
    lua_pushcfunction(L, BasePart_tostring, "__tostring"); lua_setfield(L, -2, "__tostring");

    lua_pop(L, 1);
}
