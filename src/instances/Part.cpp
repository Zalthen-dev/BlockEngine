#include "Part.h"

const char* validShapes[] = { "Block", "Sphere", "Cylinder", "Wedge", "CornerWedge", nullptr };

Part::Part(): BasePart() {
    ClassName = "Part";
    Shape = "Block";
}

Part::Part(const std::string& name,
           const Vector3Game& position,
           const Vector3Game& size,
           const Color3& color,
           bool anchored,
           std::string shape): BasePart() {
    Name = name;
    Position = position;
    Size = size;
    Color = color;
    Anchored = anchored;
    Shape = shape;
    ClassName = "Part";
}


static int Part_index(lua_State* L) {
    Part* part = (Part*)luaL_checkudata(L, 1, "PartMeta");
    const char* key = luaL_checkstring(L, 2);

    if (strcmp(key, "Shape") == 0) {
        lua_pushstring(L, part->Shape.c_str());
        return 1;
    }

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

static int Part_newindex(lua_State* L) {
    Part* part = (Part*)luaL_checkudata(L, 1, "PartMeta");
    const char* key = luaL_checkstring(L, 2);

    if (strcmp(key, "Shape") == 0) {
        const char* newShape = luaL_checkstring(L, 3);

        for (auto shape :validShapes) {
            if (strcmp(newShape, shape) == 0) {
                part->Shape = newShape;
                return 0;
            }
        }

        luaL_error(L, "attempt to set invalid Part.Shape value of '%s'", newShape);
    }

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

void Part_Bind(lua_State* L) {
    luaL_newmetatable(L, "PartMeta");

    lua_pushcfunction(L, Part_index, "__index"); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, Part_newindex, "__newindex"); lua_setfield(L, -2, "__newindex");

    lua_pop(L, 1);
}

